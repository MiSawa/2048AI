#ifndef INCLUDED_UTILS
#define INCLUDED_UTILS

#include <bits/stdc++.h>
#include <cxxabi.h>
#include <unistd.h> // sleep

#define all(v) v.begin(), v.end()
#define rall(v) v.rbegin(), v.rend()
#define sz(v) ((int)(v.size()))
#define REP(i, b, n) for(int i = (b); i < (n); ++i)
#define rep(i, n) REP(i, 0, n)
#define repsz(i, v) rep(i, sz(v))
#define pb push_back
#define eb emplace_back
#define mp make_pair
#define cauto const auto&
#define fst first
#define snd second
#define avoid(v) ((void)(v))
using namespace std;
constexpr int N = 4;

typedef long long ll;

struct P{//{{{
    int x, y;
    P() : x(0), y(0) { }
    P(int x, int y) : x(x), y(y) { }
    inline P operator+(const P &p) const { return P(x+p.x, y+p.y); }
    inline P operator-(const P &p) const { return P(x-p.x, y-p.y); }
    inline bool operator==(const P &p) const { return x == p.x && y == p.y; }
    inline bool operator!=(const P &p) const { return !(*this == p); }
    inline bool operator<(const P &p) const { return (x == p.x) ? (y < p.y) : (x < p.x); }

    inline P& operator+=(const P &p){//{{{
        x += p.x; y += p.y;
        return *this;
    }//}}}
    inline P& operator-=(const P &p){//{{{
        x -= p.x; y -= p.y;
        return *this;
    }//}}}

    friend inline P operator*(const int &n, const P &p){ return P(n*p.x, n*p.y); }
    friend inline P operator*(const P &p, const int &n){ return P(n*p.x, n*p.y); }

    friend ostream &operator<<(ostream &os, const P &p){
        return os << "(" << setw(2) << p.x << ", " << setw(2) << p.y << ")";
    }
};//}}}

struct Dir{//{{{
    P dp;
    string cmd;
    Dir() : dp(0, 0), cmd("null") {}
    Dir(P dp, string cmd) : dp(dp), cmd(cmd) {}
    operator P() const { return dp; }
    friend inline P operator*(const int &n, const Dir &dir){ return dir.dp * n; }
    friend inline P operator*(const Dir &dir, const int &n){ return dir.dp * n; }
    friend ostream &operator<<(ostream &os, const Dir &dir){ return os << dir.cmd; }
};//}}}
const array<Dir, 4> Dirs = {{//{{{
    Dir{{-1, 0}, "up"},
    Dir{{ 0,-1}, "left"},
    Dir{{ 1, 0}, "down"},
    Dir{{ 0, 1}, "right"},
}};//}}}

inline bool inRange(const int &a, const int &x, const int &b){//{{{
    return a <= x && x < b;
}//}}}
inline bool inRange(const P &a, const P &p, const P &b){//{{{
    return inRange(a.x, p.x, b.x) && inRange(a.y, p.y, b.y);
}//}}}
inline bool inside(const int &x){ return 0 <= x && x < N; }
inline bool inside(const int &x, const int &y){ return inside(x) && inside(y); }
inline bool inside(const P &p){ return inside(p.x, p.y); }

struct Row : array<int, N>{ Row(){ fill(0); } };
struct Board : array<Row, N> {//{{{
    inline const Row &operator[](const int &idx) const { return this->at(idx); }
    inline Row &operator[](const int &idx){ return this->at(idx); }
    inline int &operator[](const P &p){//{{{
        if(!inside(p)){
            cerr << "access to " << p << endl;
            abort();
        }
        return (*this)[p.x][p.y];
    }//}}}
    inline const int &operator[](const P &p) const {//{{{
        if(!inside(p)){
            cout << "access to " << p << endl;
            abort();
        }
        return (*this)[p.x][p.y];
    }//}}}

    friend istream &operator>>(istream &is, Board &b){//{{{
        rep(i, N) rep(j, N){
            int k; cin >> k; k >>= 1;
            for(b[i][j] = 0; k; k >>= 1) ++b[i][j];
        }
        return is;
    }//}}}
    friend ostream &operator<<(ostream &os, const Board &b){//{{{
        rep(i, N){
            rep(j, N){
                if(b[i][j]) os << setw(5) << (1<<b[i][j]);
                else        os << setw(5) << "-";
            }
            os << endl;
        }
        return os;
    }//}}}
};//}}}

namespace U{//{{{
    template<typename It> int arg_max(It b, It e){
        return distance(b, max_element(b, e)); }
    template<typename It> int arg_min(It b, It e){
        return distance(b, min_element(b, e)); }
    template<typename It, typename Cmp> int arg_max(It b, It e, Cmp cmp){
        return distance(b, max_element(b, e, cmp)); }
    template<typename It, typename Cmp> int arg_min(It b, It e, Cmp cmp){
        return distance(b, min_element(b, e, cmp)); }

    // zip with index//{{{
    template <typename T>
        struct iterator_extractor { typedef typename T::iterator type; };
    template <typename T>
        struct iterator_extractor<T const> { typedef typename T::const_iterator type; };
    template <typename T> struct Indexer {//{{{
        struct iterator {
            typedef typename iterator_extractor<T>::type inner_iterator;

            typedef typename std::iterator_traits<inner_iterator>::reference inner_reference;
            struct reference{
                size_t &index, idx;
                inner_reference &value, val;
                reference(size_t &index, inner_reference &value) :
                    index(index), idx(index), value(value), val(value){}
            };

            iterator(inner_iterator it): _pos(0), _it(it) {}

            reference operator*() const { return reference(_pos, *_it); }

            iterator &operator++() { ++_pos; ++_it; return *this; }
            iterator operator++(int) { iterator tmp(*this); ++*this; return tmp; }

            bool operator==(iterator const &it) const { return _it == it._it; }
            bool operator!=(iterator const &it) const { return !(*this == it); }

            private:
            size_t _pos;
            inner_iterator _it;
        };

        Indexer(T &t): _container(t) {}

        iterator begin() const { return iterator(_container.begin()); }
        iterator end() const { return iterator(_container.end()); }

        private:
        T &_container;
    }; // class Indexer//}}}
    template <typename T>
        Indexer<T> with_index(T &t) { return Indexer<T>(t); }
    //}}}

    struct stringbuilder{//{{{
        std::stringstream ss;
        template<typename T>
            stringbuilder &operator<<(const T &data){
                ss << data;
                return *this;
            }
        operator std::string(){ return ss.str(); }
    };//}}}
    typedef stringbuilder sb;

    struct formatter{//{{{
        struct{ } var;
        struct{//{{{
            void operator()(){ fm->_done(); }
            formatter* fm;
        } done;//}}}
        struct{//{{{
            formatter &operator()(){ return fm->_rewind(); }
            formatter* fm;
        } rewind;//}}}

        private:
        vector<ostringstream*> oss;
        ostringstream crr;
        bool done_flg;
        int ptr = 0;

        public:
        formatter(): oss(1, new ostringstream()), done_flg(false){//{{{
            done.fm = rewind.fm = this;
        }//}}}
        string get() const { return crr.str(); }
        operator std::string() const { return get(); }
        formatter &operator<<(const decltype(formatter::var) &x){//{{{
            assert(!done_flg);
            oss.push_back(new ostringstream());
            return *this;
        }//}}}
        formatter &operator<<(const decltype(formatter::done) &x){//{{{
            assert(!done_flg);
            done();
            return *this;
        }//}}}
        formatter &operator<<(const decltype(formatter::rewind) &x){//{{{
            assert(done_flg);
            _rewind();
            return *this;
        }//}}}
        template<typename T> formatter &operator<<(const T &x){//{{{
            if(done_flg){
                crr << x;
                if(ptr < sz(oss)){
                    crr << oss[ptr]->str();
                    crr.copyfmt(*oss[ptr++]);
                }
            }else{
                *oss.back() << x;
            }
            return *this;
        }//}}}
        formatter &operator<<(ostream &(*pf)(ostream&)){//{{{
            if(done_flg){
                crr << pf;
                if(ptr < sz(oss)){
                    crr << oss[ptr]->str();
                    crr.copyfmt(*oss[ptr++]);
                }
            }else{
                *oss.back() << pf;
            }
            return *this;
        }//}}}
        friend ostream &operator<<(ostream &os, const formatter &fm){
            return os << fm.get();
        }
        private:
        void _done(){ done_flg = true; _rewind(); }
        formatter &_rewind(){//{{{
            assert(done_flg);
            crr.clear();
            ptr = 0;
            crr.str("");
            crr << oss[ptr]->str();
            crr.copyfmt(*oss[ptr++]);
            return *this;
        }//}}}

    };//}}}

    template<typename T> struct wrap_itr :
        public iterator<bidirectional_iterator_tag, T>{//{{{
            typedef wrap_itr<T> It;
            T t;
            wrap_itr(T t): t(t){}
            It &operator++(){++t; return *this;}
            It &operator--(){--t; return *this;}
            T operator*(){return t;}
            friend bool operator!=(const It &x, const It &y){
                return x.t != y.t;
            }
            operator T(){return t;}
            It &operator()() {return *this;}
        };//}}}
    struct range { //{{{
        typedef wrap_itr<int> iterator;
        typedef iterator It;
        It begin, end;
        range(int e) : begin(0), end(max(0, e)) {}
        range(int b, int e) : begin(b), end(e) {}
        bool include(int x){return begin <= x && x < end;}
    }; //}}}

    struct Range{//{{{
        struct iterator{//{{{
            const Range &range;
            P p;
            iterator(const Range &range) : range(range), p(P(0,0)) {}
            iterator(const Range &range, const P &p): range(range), p(p){}
            inline P operator*() const { return p; }
            inline iterator &operator++(){//{{{
                ++p.y;
                if(p.y == range.br.y){
                    p.y = range.tl.y;
                    ++p.x;
                }
                return *this;
            }//}}}
            inline bool operator==(const iterator &o)const{ return p==o.p; }
            inline bool operator!=(const iterator &o)const{ return !(*this==o); }
        };//}}}
        P tl, br;
        // (top left), (bottom right)
        iterator begin_it, end_it;
        Range(const int &x, const int &y)
            : tl(P(0, 0)), br(P(x, y)),
            begin_it(*this, tl), end_it(*this, P(br.x, tl.y)){}
        Range(const P &br):
            tl(P(0, 0)), br(br),
            begin_it(*this, tl), end_it(*this, P(br.x, tl.y)){}
        Range(const P &tl, const P &br):
            tl(tl), br(br),
            begin_it(*this, tl), end_it(*this, P(br.x, tl.y)){}
        Range(const Range &o):
            tl(o.tl), br(o.br),
            begin_it(*this, tl), end_it(*this, P(br.x, tl.y)){}
        Range(const int &n)
            : tl(P(0, 0)), br(P(n, n)),
            begin_it(*this, tl), end_it(*this, P(br.x, tl.y)){}
        inline Range &operator=(const Range &o){//{{{
            tl = o.tl; br = o.br;
            begin_it.p = tl; end_it.p = P(br.x, tl.y);
            return *this;
        }//}}}
        inline iterator begin(){ return begin_it; }
        inline iterator end(){ return end_it; }
        inline const iterator begin() const { return begin_it; }
        inline const iterator end() const { return end_it; }
        inline bool include(const P &p) const {//{{{
            return tl.x <= p.x && p.x < br.x
                && tl.y <= p.y && p.y < br.y;
        }//}}}
        inline bool include(const Range &range) const {//{{{
            for(cauto p : range) if(!include(p)) return false;
            return true;
        }//}}}
        inline bool eol(const P &p) const { return p.y == br.y - 1; }
        inline Range intersection(const Range &range) const {//{{{
            return Range(P(max(tl.x, range.tl.x), max(tl.y, range.tl.y)),
                    P(min(br.x, range.br.x), min(br.y, range.br.y)));
        }//}}}
    };//}}}

#include<cxxabi.h>
    template<typename T> string classname(){//{{{
        char *p;
        int status = 0;
        p = abi::__cxa_demangle(typeid(T).name(), 0, 0, &status);
        string res = p;
        free(p);
        return res;
    }//}}}

    struct Timer{//{{{
        typedef std::chrono::high_resolution_clock hrclock;
        static constexpr double ClocksPerSec = std::chrono::seconds(1) / hrclock::duration(1);
        hrclock::time_point t;
        Timer() { reset(); }
        void reset(){ t = hrclock::now(); }
        double getTime() const { return (hrclock::now()-t).count()/ClocksPerSec; }
        friend ostream &operator<<(ostream &os, const Timer &timer){
            char c[100];
            sprintf(c, "%.3f seconds", timer.getTime());
            return os << c;
        }
        void dump(){ cout << *this << endl; }
    };//}}}
}//}}}

using namespace U;

namespace Random{//{{{
    unsigned int x=123456789,y=362436069,z=521288629,w=88675123;
    void seed(unsigned int _w){
        x=123456789,y=362436069,z=521288629,w=_w;
    }
    unsigned int xor128(){//{{{
        unsigned int t=x^(x<<11);
        x=y;y=z;z=w;
        return w=(w^(w>>19))^(t^(t>>8));
    }//}}}
    int ni(int m){ return xor128() % m; }

    template<typename It, typename T = typename std::iterator_traits<It>::value_type>//{{{
        vector<T> sample(const It &b, const It &e, int k){
            int n = distance(b, e);
            vector<T> res;
            if(k == 1){
                res.eb(b[Random::ni(n)]);
            }else if(k == 2){
                int i = Random::ni(n), j = Random::ni(n-1);
                if(j >= i) ++j;
                res.eb(b[i]); res.eb(b[j]);
            }else{
                res.assign(b, e);
                random_shuffle(all(res));
                res.resize(k);
            }
            return res;
        };//}}}
};//}}}


struct State{//{{{
    Board board;
    int score;
    State() : score(0) { }
    State(const State &s) : board(s.board), score(s.score) { }
    inline bool can_move(const Dir& dir) const {//{{{
        for(cauto p : Range(N)) if(board[p]){
            auto q = p + dir;
            if(!inside(q)) continue;
            if(board[q] == 0 || board[q] == board[p]) return true;
        }
        return false;
    }//}}}
    inline void move(const Dir& dir){//{{{
        for(cauto p : Range(N)) if(!inside(p + dir)){
            array<int, N> line = {};
            int cnt = 0;
            for(cauto i : range(N)){
                line[cnt] = board[p - dir * i];
                board[p - dir * i] = 0;
                if(line[cnt]) ++cnt;
            }
            for(cauto i : range(min(cnt-1, N-1))) if(line[i] == line[i+1]){
                ++line[i]; line[i+1] = 0;
                score += 1<<line[i];
            }
            cnt = 0;
            for(cauto i : range(N)) if(line[i]) board[p - dir * cnt++] = line[i];
        }
    }//}}}
    inline State getMoved(const Dir& dir) const {//{{{
        State res = *this;
        res.move(dir);
        return res;
    }//}}}
    inline bool lose() const {//{{{
        for(cauto p : Range(N)){
            if(!board[p]){
                return false;
            }else{
                for(cauto dir : range(2))
                    if(inside(p+Dirs[dir]) && board[p] == board[p+Dirs[dir]])
                        return false;
            }
        }
        return true;
    }//}}}
    inline int &operator[](const P &p){ return board[p]; }
    inline const int &operator[](const P &p) const { return board[p]; }
    friend istream& operator>>(istream& is, State& state){//{{{
        return is >> state.board;
    }//}}}
    friend ostream& operator<<(ostream& os, const State& state){//{{{
        return os << state.board;
    }//}}}
};//}}}
struct Game{//{{{
    Game(){ for(cauto _ : range(2)) avoid(_), gen_tile(); }
    Game(const State &state) : state(state) { }
    inline void gen_tile(){//{{{
        static array<P, N*N> ps;
        int cnt = 0;
        for(cauto p : Range(N)) if(!state[p])
            ps[cnt++] = p;
        state[ps[Random::ni(cnt)]] = (Random::ni(10) ? 1 : 2);
    }//}}}
    inline void move(const Dir &dir){ state.move(dir); }
    inline bool can_move(const Dir &dir) const { return state.can_move(dir); }
    inline bool lose() const { return state.lose(); }
    inline int score() const { return state.score; }
    inline int &operator[](const P &p) { return state[p]; }
    inline const int &operator[](const P &p) const { return state[p]; }
    inline State getState() const { return State(state); }
    friend ostream& operator<<(ostream& os, const Game& game){//{{{
        return os << game.state.score << endl << game.state << endl;
    }//}}}
    private:
    State state;
};//}}}

#endif

#if __INCLUDE_LEVEL__ == 0
int main(void){
    return 0;
}
#endif

// vim:set foldmethod=marker commentstring=//%s:
