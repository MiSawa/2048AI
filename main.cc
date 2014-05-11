#include "./utils.cc"

inline double sigmoid(const double &x){ return 1 / (1 + exp(-x)); }

const array<int, 2> GenTiles = {{ 1, 2 }};
namespace UCT{//{{{
    const int T = 40;
    double playout_win(Game &game, int turn_offset){//{{{
        int cnt = turn_offset;
        while(!game.lose()){
            ++cnt;
            Dir dir;
            do{
                dir = Dirs[Random::ni(4)];
            }while(!game.can_move(dir));
            game.move(dir);
            game.gen_tile();
            if(cnt > 65) break;
        }
        return sigmoid(0.7*(cnt - 30)); //(double)(cnt) / 11;
    }//}}}

    struct Node {//{{{
        typedef Node *Ptr;
        int n;
        vector<Ptr> ch;
        vector<double> win;
        vector<int> cnt;
        int sum;
        int depth;
        Node(int n, int depth)
            : n(n), ch(n, (Ptr)(0)), win(n), cnt(n), sum(0), depth(depth) {}
        virtual ~Node(){ for(cauto p : ch) if(p) delete p; }
        inline int arg_max_ucb(){//{{{
            vector<double> ucb(n);
            for(cauto i : range(n)){
                if(!cnt[i]) return i;
                ucb[i] = (double)win[i] / cnt[i];
                ucb[i] += sqrt(2*log(sum) / cnt[i]);
            }
            return arg_max(all(ucb));
        }//}}}
        inline double get_avg(int i) const { return (double)win[i] / cnt[i]; }
        inline int arg_max_avg() const {//{{{
            vector<double> avg(n);
            for(cauto i : range(n)) if(cnt[i]) avg[i] = get_avg(i);
            return arg_max(all(avg));
        }//}}}
        virtual double run(Game &) { return true; }
        inline int size(){//{{{
            int res = 1;
            for(cauto p : ch) if(p) res += p->size();
            return res;
        }//}}}
    };//}}}
    struct NodeFriend;
    struct NodeEnemy;
    struct NodeEnemy : Node{//{{{
        vector<tuple<P, int>> valid;
        NodeEnemy(const Game &game, int depth) : Node(2*N*N, depth){
            for(cauto p : Range(N)) if(!game[p]) for(cauto tile : GenTiles)
                valid.eb(p, tile);
            n = valid.size();
        }
        inline double run(Game &);
    };//}}}
    struct NodeFriend : Node{//{{{
        vector<Dir> valid;
        NodeFriend(const Game &game, int depth) : Node(4, depth){
            for(cauto i : range(4)) if(game.can_move(Dirs[i]))
                valid.eb(Dirs[i]);
            n = valid.size();
        }
        inline double run(Game &);
    };//}}}
    inline double NodeFriend::run(Game &game){//{{{
        if(n == 0) return false;
        int t = arg_max_ucb();
        game.move(valid[t]);
        double res;
        ++sum;
        if(cnt[t]++ < T){
            game.gen_tile();
            res = playout_win(game, depth/2);
        }else{
            if(!ch[t]) ch[t] = new NodeEnemy(game, depth+1);
            res = 1 - ch[t]->run(game);
        }
        win[t] += res;
        return res;
    }//}}}
    inline double NodeEnemy::run(Game &game){//{{{
        if(n == 0) return true;
        int t = arg_max_ucb();
        game[get<0>(valid[t])] = get<1>(valid[t]);
        double res;
        ++sum;
        if(cnt[t]++ < T){
            res = 1 - playout_win(game, depth/2);
        }else{
            if(!ch[t]) ch[t] = new NodeFriend(game, depth+1);
            res = 1 - ch[t]->run(game);
        }
        win[t] += res;
        return res;
    }//}}}

    const int ITERMAX = 100000;
    Dir decide(const State &state){//{{{
        Game game(state);
        NodeFriend root(game, 0);
        tuple<int, double> memo(-1, -1);
        auto runner = [&](double time){//{{{
            Timer t;
            rep(__, ITERMAX/100){
                rep(_, 100){
                    game = Game(state);
                    root.run(game);
                }
                if(t.getTime() > time) break;
            }
        };//}}}
        //double time_count = 0.2;
        runner(0.8);
        int d = root.arg_max_avg();
        //time_count += sqrt(1.1 - root.get_avg(d));
        //runner(sqrt(1.1 - root.get_avg(d)));
        //d = root.arg_max_avg();
        cerr << "p = " << root.get_avg(d) << "  (";
        cerr << root.size() << " nodes / " << root.sum << " iterations)" << endl;
        //cerr << "  find in " << get<1>(memo) << " / " << time_count << " sec" << endl;
        return root.valid[d];
    }//}}}
};//}}}

int main(int argc, char const *argv[]){
    Random::seed(1);
    for(int cont; cin >> cont, cont; ){
        int score;
        State state;
        cin >> score >> state;
        state.score = score;
        Dir res = UCT::decide(state);
        cerr << state << endl;
        cerr << res << endl;
        cout << res << endl;
        cout.flush();
    }
    return 0;
}
// vim:set foldmethod=marker commentstring=//%s:
