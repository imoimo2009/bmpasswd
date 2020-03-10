#include <stdio.h>
#include <stdlib.h>

#define PW_NUM          16
#define PW_LENGTH       20
#define SCSTR_LENGTH    8

// 加算パターン配列INDEX
enum eId {
    id_bs,
    id_01h,
    id_rc,
    id_s01,
    id_1hm,
    id_01k,
    id_bp,
    id_1hk,
    id_fm,
    id_bn,
    id_01m,
    id_rs,
    id_10m,
    id_10k,
    id_ov,
    id_s16,
    id_wt
};

// 入力値配列INDEX
enum enId {
    n_bp,
    n_bn,
    n_rs,
    n_rc,
    n_fm,
    n_ov,
    n_wt,
    n_sc,
    n_st
};

//加算パターン配列
int pw[][20]  = {
    { 9, 0, 9, 0, 9, 0, 9, 0, 9, 0, 9, 0, 9, 0, 9, 0, 9, 0, 9, 0}, //00 bs
    { 1,-1, 1,-1, 2,-2, 2,-2, 2,-2, 2,-2, 2,-2, 2,-2, 2,-2, 2, 0}, //01 01h
    { 0, 8,-8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //02 rc
    { 0, 0, 1,-1, 2,-2, 2,-2, 2,-2, 2,-2, 2,-2, 2,-2, 2,-2, 2, 0}, //03 s01
    { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2}, //04 1hm
    { 0, 0, 0, 0, 0, 1,-1, 1,-1, 2,-2, 2,-2, 2,-2, 2,-2, 2,-2, 4}, //05 01k
    { 0, 0, 0, 0, 0, 0, 1,-1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2}, //06 bp
    { 0, 0, 0, 0, 0, 0, 0, 1,-1, 2,-2, 2,-2, 2,-2, 2,-2, 2,-2, 4}, //07 1hk
    { 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //08 fm
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,-1, 1,-1, 2,-2, 2,-2, 2, 0}, //09 bn
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,-1, 1, 0, 0, 0, 0, 0, 2}, //10 01m
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8,-8, 0, 0, 0, 0, 0, 0}, //11 rs
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2}, //12 10m
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,-1, 1,-1, 2}, //13 10k
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,-1, 1, 0}, //14 ov
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,-1, 2}, //15 s16
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}  //16 wt
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
    char msg[][255] = {"bp","bn","rs","rc","fm","ov","wt","sc","st"};

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
