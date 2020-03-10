#include <stdio.h>
#include <stdlib.h>

#define PW_NUM          16
#define PW_LENGTH       20
#define SCSTR_LENGTH    8

// 加算パターン配列INDEX
enum eId {
    id_bs,  //00 初期値
    id_01h, //01 スコア百の位
    id_rc,  //02 リモコン
    id_s01, //03 ステージ1~15
    id_1hm, //04 スコア10万の位
    id_01k, //05 スコア億の位
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
    { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2}, //04 スコア10万の位
    { 0, 0, 0, 0, 0, 1,-1, 1,-1, 2,-2, 2,-2, 2,-2, 2,-2, 2,-2, 4}, //05 スコア億の位
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
    int si[] = {id_1hm,id_10m,id_01m,id_1hk,id_10k,id_01k,id_01h};

    for(i = 0;i < 7;i++){
        s[i] = '0' + (char)a[si[i]];
    }
    s[7] = '\0';
    return atoi(s);
}

//パスワードをデコードし、パラメータを表示する
int decode_pw(int *p){
    int i,t;
    int a[17];
    char s[8];
    int x[] = { 0, 0, 1, 2, 3, 5, 6, 7, 8,10,11,12,13,15,16,17,18};
    int d[] = { 0, 1, 8, 1, 1, 1, 1, 1, 8, 1, 1, 8, 1, 1, 1, 1, 1};

    for(i = 1;i <= PW_NUM;i++){
        a[i] = 0;
        t = get_diff(p,x[i]);
        if(t > 0){
            add_idx(p,pw[i],-(t / d[i]));
            a[i] = t;
        }
    }
    printf(" bp= %2d  ",a[id_bp]);
    printf(" bn= %2d  ",a[id_bn]);
    printf(" rs= %2d  ",a[id_rs]/8);
    printf(" rc= %2d\n",a[id_rc]/8);
    printf(" fm= %2d  ",a[id_fm]/8);
    printf(" ov= %2d  ",a[id_ov]);
    printf(" wt= %2d\n",a[id_wt]);
    printf("score= %d\n",mk_score(a));
    printf("stage= %d\n",a[id_s16]*16 + a[id_s01]);
    return 0;
}

//パラメータからパスワードのエンコードを行う
void encode_pw(int *p,int *n){
    int i;
    char s[41];
    char sc_str[8];

    for(i = 0;i < 20;i++) p[i] = pw[id_bs][i];
    add_idx(p,pw[id_bp],n[n_bp]);
    add_idx(p,pw[id_bn],n[n_bn]);
    add_idx(p,pw[id_rs],n[n_rs]);
    add_idx(p,pw[id_rc],n[n_rc]);
    add_idx(p,pw[id_fm],n[n_fm]);
    add_idx(p,pw[id_ov],n[n_ov]);
    add_idx(p,pw[id_wt],n[n_wt]);
    sprintf(sc_str,"%07d",n[n_sc]);
    add_idx(p,pw[id_01h],c2i(sc_str[6]));
    add_idx(p,pw[id_01k],c2i(sc_str[5]));
    add_idx(p,pw[id_10k],c2i(sc_str[4]));
    add_idx(p,pw[id_1hk],c2i(sc_str[3]));
    add_idx(p,pw[id_01m],c2i(sc_str[2]));
    add_idx(p,pw[id_10m],c2i(sc_str[1]));
    add_idx(p,pw[id_1hm],c2i(sc_str[0]));
    add_idx(p,pw[id_s16],n[n_st] / 16);
    add_idx(p,pw[id_s01],n[n_st] % 16);
}

//パラメータを入力
void get_param(int *n){
    int i;
    char g[255];
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

    for(i = 0;i < 9;i++){
        printf("? %s=",msg[i]);
        fgets(g,255,stdin);
        n[i] = atoi(g);
    }
}

//エントリポイント
int main(int argc,char *argv[]){
    int p[20],n[10];

    get_param(n);
    encode_pw(p,n);
    print_pw(p);
    decode_pw(p);
}
