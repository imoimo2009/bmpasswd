#include <stdio.h>
#include <stdlib.h>

#define PW_NUM          16
#define PW_LENGTH       20
#define PARAM_NUM       9
#define SCORE_LENGTH    7
#define STAGE_LENGTH    2
#define IDX_MAX         15

// 加算パターン配列INDEX
enum eId {
    id_bs,  //00 初期値
    id_01h, //01 スコア百の位
    id_rc,  //02 リモコン
    id_s01, //03 ステージ1~15
    id_1hm, //04 スコア億の位
    id_01k, //05 スコア千の位
    id_bp,  //06 爆風の強さ
    id_1hk, //07 スコア10万の位
    id_fm,  //08 ファイアーマン
    id_bn,  //09 爆弾の数
    id_01m, //10 スコア100万の位
    id_rs,  //11 ローラースケート
    id_10m, //12 スコア千万の位
    id_10k, //13 スコア万の位
    id_ov,  //14 ドア・アイテム表示
    id_s16, //15 ステージ16毎
    id_wt   //16 壁すり抜け
};

// 入力値配列INDEX
enum enId {
    n_bp, //00 爆風の強さ
    n_bn, //01 爆弾の数
    n_rs, //02 ローラースケート
    n_rc, //03 リモコン
    n_fm, //04 ファイアーマン
    n_ov, //05 ドア・アイテム表示
    n_wt, //06 壁すり抜け
    n_sc, //07 スコア
    n_st  //08 ステージ
};

//加算パターン配列
int pw[][20]  = {
    { 9, 0, 9, 0, 9, 0, 9, 0, 9, 0, 9, 0, 9, 0, 9, 0, 9, 0, 9, 0}, //00 初期値
    { 1,-1, 1,-1, 2,-2, 2,-2, 2,-2, 2,-2, 2,-2, 2,-2, 2,-2, 2, 0}, //01 スコア百の位
    { 0, 8,-8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //02 リモコン
    { 0, 0, 1,-1, 2,-2, 2,-2, 2,-2, 2,-2, 2,-2, 2,-2, 2,-2, 2, 0}, //03 ステージ1~15
    { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2}, //04 スコア億の位
    { 0, 0, 0, 0, 0, 1,-1, 1,-1, 2,-2, 2,-2, 2,-2, 2,-2, 2,-2, 4}, //05 スコア千の位
    { 0, 0, 0, 0, 0, 0, 1,-1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2}, //06 爆風の強さ
    { 0, 0, 0, 0, 0, 0, 0, 1,-1, 2,-2, 2,-2, 2,-2, 2,-2, 2,-2, 4}, //07 スコア10万の位
    { 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //08 ファイアーマン
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,-1, 1,-1, 2,-2, 2,-2, 2, 0}, //09 爆弾の数
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,-1, 1, 0, 0, 0, 0, 0, 2}, //10 スコア100万の位
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8,-8, 0, 0, 0, 0, 0, 0}, //11 ローラースケート
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2}, //12 スコア千万の位
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,-1, 1,-1, 2}, //13 スコア万の位
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,-1, 1, 0}, //14 ドア・アイテム表示
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,-1, 2}, //15 ステージ16毎
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}  //16 壁すり抜け
};

//各項目の開始位置
int pwx[] = { 0, 0, 1, 2, 3, 5, 6, 7, 8,10,11,12,13,15,16,17,18};

//各項目の値1あたりの増分
int pwd[] = { 0, 1, 8, 1, 1, 1, 1, 1, 8, 1, 1, 8, 1, 1, 1, 1, 1};

int pwi[] = {id_bp,id_bn,id_rs,id_rc,id_fm,id_ov,id_wt};
int sci[] = {id_1hm,id_10m,id_01m,id_1hk,id_10k,id_01k,id_01h};
int sti[] = {id_s16,id_s01};

//入出力メッセージ配列
char msg[][255] = {
    "爆風の強さ",
    "爆弾の数",
    "ローラースケート",
    "リモコン",
    "ファイアーマン",
    "ドア・アイテム表示",
    "壁すり抜け",
    "スコア",
    "ステージ"
};

//文字パターン
char *pw_char = "AOFKCPGELBHMJDNI";

//文字からINDEXを取得
int get_idx(char c){
    int i;

    for(i = 0;i < 16;i++){
        if(pw_char[i] == c) return i;
    }
    return -1;
}

//INDEXから文字を取得
char get_char(int i){
    return pw_char[i];
}

//数値を0~15の範囲に変換する
int ring(int n){

    n = n % 16;
    if(n < 0) n = 16 + n;
    return n;
}

//加算パターンに沿って各桁を加算する
void add_idx(int *p,int * d,int n){
    int i;

    for(i = 0;i < 20;i++){
        p[i] += d[i] * n;
        p[i] = ring(p[i]);
    }
}

//文字を数値に変換
int c2i(char c){
    return (int)c - '0';
}

//原点からの差分を所得
int get_diff(int *p,int i){
    int a = p[i] - pw[id_bs][i];

    if(a < 0) a += 16;
    return a;
}

//パスワードを表示する
void print_pw(int *p){
    int i;
    char s[41];

    for(i = 0;i < 20;i++){
        s[i * 2] = pw_char[p[i]];
        s[i * 2 + 1] = ' ';
    }
    s[40] = '\0';
    printf("%s\n",s);
}

//スコア各桁を合成し、intで返す
int mk_score(int *a){
    int i;
    char s[8];

    for(i = 0;i < 7;i++){
        s[i] = '0' + (char)a[sci[i]];
    }
    s[7] = '\0';
    return atoi(s);
}

//パスワードをデコードする
int decode_pw(int *p,int *a){
    int i,t,c;

    for(i = 1;i <= PW_NUM;i++){
        a[i] = 0;
        t = get_diff(p,pwx[i]);
        if(t > 0){
            add_idx(p,pw[i],-(t / pwd[i]));
            a[i] = t;
        }
    }
    c = 0;
    for(i = 0;i < 20;i++){
        if(p[i] == pw[id_bs][i]) c++;
    }
    if(c == 20) return 1;
    return 0;
}

//パラメータからパスワードのエンコードを行う
void encode_pw(int *p,int *n){
    int i,j;
    char sc_str[SCORE_LENGTH + 1];

    for(i = 0;i < 20;i++) p[i] = pw[id_bs][i];
    for(i = 0;i < PARAM_NUM;i++){
        if(i <= n_wt){
            add_idx(p,pw[pwi[i]],n[i]);
        }else if(i == n_sc){
            sprintf(sc_str,"%07d",n[i]);
            for(j = 0;j < SCORE_LENGTH;j++){
                add_idx(p,pw[sci[j]],c2i(sc_str[j]));
            }
        }else{
            add_idx(p,pw[id_s16],n[i] / 16);
            add_idx(p,pw[id_s01],n[i] % 16);
        }
    }
}

//パラメータを入力
void get_param(int *n){
    int i;
    char g[255];

    for(i = 0;i < PARAM_NUM;i++){
        printf("%s ?",msg[i]);
        fgets(g,255,stdin);
        n[i] = atoi(g);
    }
}

//パラメータを表示
void print_param(int *a){
    int i;

    for(i = 0;i < PARAM_NUM;i++){
        if(i <= n_wt){
            printf("%s = %2d\n",msg[i],a[pwi[i]] / pwd[pwi[i]]);
        }else if(i == n_sc){
            printf("%s = %d\n",msg[i],mk_score(a));
        }else{
            printf("%s = %d\n",msg[i],a[id_s16]*16 + a[id_s01]);
        }
    }
}

//エントリポイント
int main(int argc,char *argv[]){
    int p[PW_LENGTH];
    int n[PARAM_NUM];
    int a[PW_NUM];

    get_param(n);
    encode_pw(p,n);
    print_pw(p);
    if(decode_pw(p,a)){
        print_param(a);
    }else{
        print_pw(p);
    }
}
