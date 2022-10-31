#include <limits.h>
#include <assert.h>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

int BFS(vector<vector<pair<int, int>>> &graph, int s, int t,
        vector<pair<int, int>> &prev, const vector<int> &rescap)
{
  vector<bool> visited = vector<bool>(graph.size(), false);
  vector<int> minima = vector<int>(graph.size(), 0);
  queue<int> q;
  q.push(s);
  minima[s] = INT_MAX;
  visited[0] = true;
  while (q.size() > 0)
  {
    int u = q.front();
    int f = minima[u];
    q.pop();
    for (pair<int, int> p : graph[u])
    {
      int v = p.first;
      int idx = p.second;
      if (!visited[v] && rescap[idx] > 0)
      {
        int new_f = min(f, rescap[idx]);
        q.push(v);
        minima[v] = new_f;
        visited[v] = true;
        prev[v] = {u, idx};
        if (v == t)
        {
          return minima[v];
        }
      }
    }
  }
  return 0;
}

int main()
{
    // sample parsing code
    int num_friends = 0;
    int num_vals = 0;
    int num_colors = 0;
    assert(scanf("%d %d %d\n", &num_friends, &num_vals, &num_colors) == 3);

    //cout << "Friends: " << num_friends << endl;
    //cout << "Vals:    " << num_vals    << endl;
    //cout << "Colors:  " << num_colors  << endl;

    fprintf(stderr, "Friends: %d, vals: %d, colors: %d\n", num_friends, num_vals, num_colors);

    pair<int, int> card_exists[num_friends][num_vals][num_colors];
    
    int num_vertices = 2;
    int num_edges = 0;

    int s = 0;
    int t;
    int next_player;

    int edge_hd, edge_tl;
    
    for(int f = 0; f < num_friends; f++) {
        for(int v = 0; v < num_vals; v++) {
            for(int c = 0; c < num_colors; c++) {
                card_exists[f][v][c].first = -1;
                card_exists[f][v][c].second = 0;
            }
        }
    }

    int cur_vert_index = 1;
    int edge_index = 0;

    for (int i = 0; i < num_friends; i++)
    {

        int x;
        int c;

        while (cin.peek() != '\n' && cin.peek() != -1)
        {
            assert(scanf("%d %d", &x, &c) == 2);
            fprintf(stderr, "Friend %d has card with value %d, color %d\n", i + 1, x, c);

            //cout << "Add card " << i << " " << x-1 << " " << c-1 << endl;

            if(card_exists[i][x-1][c-1].first == -1) {
                card_exists[i][x-1][c-1].first = cur_vert_index;
                card_exists[i][x-1][c-1].second = 1;

                cur_vert_index += 2;
                num_vertices += 2;
            } else {
                card_exists[i][x-1][c-1].second++;
            }
        
        }
        cin.get();
    }

    //cout << "Finished reading cards" << endl;

    vector<int> cap; //capacity
    auto graph = vector<vector<pair<int, int> > >(2*num_vertices + 2, vector <pair<int, int>>());

    for(int f = 0; f < num_friends; f++) {
        for(int v = 0; v < num_vals; v++) {
            for(int c = 0; c < num_colors; c++) {

                if(card_exists[f][v][c].first != -1) {

                    //cout << "Checking f: " << f << ", v: " << v << ", c: " << c << endl;

                    graph[card_exists[f][v][c].first  ].push_back({card_exists[f][v][c].first+1, edge_index++});
                    graph[card_exists[f][v][c].first+1].push_back({card_exists[f][v][c].first  , edge_index++});
                    cap.push_back(card_exists[f][v][c].second);
                    cap.push_back(0);
                    num_edges++;

                    next_player = (f + 1) % num_friends;

                    for(int x = 0; x < num_colors; x++) {
                        if(x != c) {
                            if(card_exists[next_player][v][x].first != -1) {
                                edge_hd = card_exists[f][v][c].first + 1;
                                edge_tl = card_exists[next_player][v][x].first;

                                graph[edge_hd].push_back({edge_tl, edge_index++});
                                graph[edge_tl].push_back({edge_hd, edge_index++});
                                cap.push_back(card_exists[f][v][c].second);
                                cap.push_back(0);
                                num_edges++;
                            }
                        }
                    }

                    if((v != (num_vals - 1)) && card_exists[next_player][v+1][c].first != -1) {
                        edge_hd = card_exists[f][v][c].first + 1;
                        edge_tl = card_exists[next_player][v+1][c].first;

                        graph[edge_hd].push_back({edge_tl, edge_index++});
                        graph[edge_tl].push_back({edge_hd, edge_index++});
                        cap.push_back(card_exists[f][v][c].second);
                        cap.push_back(0);
                        num_edges++;
                    }

                    for(int x = 0; x < num_colors; x++) {
                        if(x != c) {
                            if(card_exists[f][v][x].first != -1) {
                                edge_hd = card_exists[f][v][c].first + 1;
                                edge_tl = card_exists[f][v][x].first;

                                graph[edge_hd].push_back({edge_tl, edge_index++});
                                graph[edge_tl].push_back({edge_hd, edge_index++});
                                cap.push_back(card_exists[f][v][c].second);
                                cap.push_back(0);
                                num_edges++;
                            }
                        }
                    }

                    if((v != (num_vals - 1)) && card_exists[f][v+1][c].first != -1) {
                        edge_hd = card_exists[f][v][c].first + 1;
                        edge_tl = card_exists[f][v+1][c].first;

                        graph[edge_hd].push_back({edge_tl, edge_index++});
                        graph[edge_tl].push_back({edge_hd, edge_index++});
                        cap.push_back(card_exists[f][v][c].second);
                        cap.push_back(0);
                        num_edges++;
                    }
                }

                // note that forwards edge is directly followed by backwards edge, this is required
                // graph[u].push_back({v, edge_index++});
                // graph[v].push_back({u, edge_index++});
                // cap.push_back(c);
                // cap.push_back(0);
            }
        }
    }

    //cout << "Finished building main graph" << endl;

    // add code for linking s and t to graph
    t = cur_vert_index;

    // add edge from s to every v=1
    for(int f = 0; f < num_friends; f++) {
        for(int c = 0; c < num_colors; c++) {
            if(card_exists[f][0][c].first != -1) {
                edge_hd = s; 
                edge_tl = card_exists[f][0][c].first;

                graph[edge_hd].push_back({edge_tl, edge_index++});
                graph[edge_tl].push_back({edge_hd, edge_index++});
                cap.push_back(card_exists[f][0][c].second);
                cap.push_back(0);
                num_edges++;
            }

            if(card_exists[f][num_vals-1][c].first != -1) {
                edge_hd = card_exists[f][num_vals-1][c].first + 1;
                edge_tl = t;

                graph[edge_hd].push_back({edge_tl, edge_index++});
                graph[edge_tl].push_back({edge_hd, edge_index++});
                cap.push_back(card_exists[f][num_vals-1][c].second);
                cap.push_back(0);
                num_edges++;
            }
        }
    }

    //cout << "Finished building s & t" << endl;

    //cout << "s: " << s << ", t: " << t << ", verts: " << num_vertices << ", edges: " << num_edges << endl;

    /*
    // sample graph gen code
    // these are hard-coded into arrays only because it's easier to read off the original graph. 
    // you'll probably want to generate the vectors on the fly in your implementation without intermediate arrays.
    int edge_heads[] = { 0, 0, 2, 1, 2 };
    int edge_tails[] = { 1, 2, 1, 3, 3 };
    int edge_caps[] = { 1, 100, 1, 100, 1 };
    int num_vertices = 4;
    int num_edges = 5;

    vector<int> cap; //capacity
    auto graph = vector<vector<pair<int, int> > >(num_vertices, vector <pair<int, int>>());
    int s = 0;
    int t = 3;

    int edge_index = 0;
    for (int i = 0; i < num_edges; i++){
        int u = edge_heads[i];
        int v = edge_tails[i];
        int c = edge_caps[i];

        // note that forwards edge is directly followed by backwards edge, this is required
        graph[u].push_back({v, edge_index++});
        graph[v].push_back({u, edge_index++});
        cap.push_back(c);
        cap.push_back(0);
    }
    */


    int ans = 0;
    auto prev = vector<pair<int, int>>(graph.size(), {-1, -1});
    vector<int> rescap = vector<int>(cap);
    int f = BFS(graph, s, t, prev, rescap);
    int count = 0;

    while (f)
    {
        if (count % 100 == 0)
        fprintf(stderr, "[FF debug] iteration %d: bottleneck capacity %d\n", count++, f);
        else
        count++;
        int v = t;
        int sub_count = 1;
        while (v != s)
        {
        auto p = prev[v];
        int u = p.first;
        int idx = p.second;
        int rev_idx = (cap[idx] == 0) ? idx - 1 : idx + 1;
        rescap[idx] -= f;
        rescap[rev_idx] += f;
        v = u;
        sub_count++;
        }
        // fprintf(stderr, "%d\n", sub_count);
        ans += f;
        f = BFS(graph, s, t, prev, rescap);
    }
    printf("%d\n", ans);
}