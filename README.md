## なにこれ
[2048](http://gabrielecirulli.github.io/2048/) の AI.


## 強いの?
僕よりは.


## 必要なもの
AI 自体は C++11.
割と最近のコンパイラじゃないとダメかも.

動かすのに, firefox と ruby の 2.0 以降くらい.
多分 1.9.3 くらいでも動く気がする.

あとは `bundle install` で必要なのが入るはず.

firefox 以外が使いたければ, それっぽい所それっぽくいじれば出来る.


## どうやって動かすの
0. `Makefile` のコンパイラ部分をお好きなのに変える.
0. `$ make`
0. `$ ./run`


## 注意
自分で自分の環境で適当に動かす用に作ったのを公開したので, エラー対応とかほぼしてない.

firefox だけ閉じても死ななかったり, 終わっても firefox 閉じなかったりするけど, 仕様です.
落ちたりするとどうなるのかとか, 知らんです.

弄るなら, その辺ちょっと注意して下さい.


## AI への入力
`main.cc` 見れば解ると思うけれど, 各ターンの最初に `0` or `1` が来る.

`0` が来たら速やかに死なないといけない.

`1` の後は, 現在のスコア, 盤面が来る感じ.

