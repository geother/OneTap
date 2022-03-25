#include <iostream>
#include <cstdio>
#include "windows.h"
#include <conio.h>
#include <algorithm>
#define DB double
#define FOR(x,y,z) for(register int x=y;x<=z;++x)
#define REP(x,y,z) for(register int x=y;x>=z;--x)
using namespace std;

inline int in() {
    int x=0,w=0; char ch=0;
    while(!isdigit(ch)) {w|=ch=='-'; ch=getchar();}
    while(isdigit(ch)) x=(x<<3)+(x<<1)+(ch^48), ch=getchar();
    return w?-x:x;
}

int user_cnt, k = 1, text_cnt;
char kbd[150] = {'q','a','z','Q','A','Z','w','s','x','W','S','X','e','d','c','E','D','C','r','f','v','R','F','V','t','g','b','y','h','T','G','B','Y','H','u','j','n','i','U','J','N','I','o','k','m','O','K','M','p','l',',','P','L','<','[',';','.','{',':','>',']','\'','/','}','"','?'};
int pos_x[100] = {9,15,22,9,15,22,  9,15,22,9,15,22,    9,15,22,9,15,22,   9,15,22,9,15,22,   9,15,22,9,15,9,15,22,9,15,     9,15,22,9,9,15,22,9,     9,15,22,9,15,22,   9,15,22,9,15,22,   9,15,22,9,15,22,   9,15,22,9,15,22};
int pos_y[100] = {14,17,20,14,17,20,18,21,24,18,21,24,  22,25,28,22,25,28, 26,29,32,26,29,32, 30,33,36,34,37,30,33,36,34,37, 38,41,40,42,38,41,40,42, 46,45,44,46,45,44, 50,49,48,50,49,48, 54,53,52,54,53,52, 58,57,56,58,57,56};
int vis[100];

struct TEXT {
    char text[150];
    int len;
}txt[150];

/*用户数据*/
struct USER {
    char name[30], password[30];
    DB min_time[5] = {1e7,1e7,1e7,1e7}, max_rate[5] = {0.0,0.0,0.0,0.0}, tpp[5] = {0.0,0.0,0.0,0.0};
    int max_score = 0;
    // int id;
}user[500];


//函数一览
void HALL(int _k);
void CUR_GO(short x, short y);
void CAL_TPP(int _k);
DB CAL_RATE (int _x, int _y);
int CAL_MID (int _x);
DB NOW();
void SET_COLOR(int _bcolor, int _foclor);
void RELAX ();
void ANTI_GOD_OPT();
void HIDE_CONSOLE_CURSOR();
void SET_TITLE(LPCSTR lpTitle);
void PROJECT_INIT();
void COVER_SPACE (short _X, short _Y, short __X, short __Y);
void PRT_SCORE (DB _t, DB _rate);
void TRANS (int _T);
void LAUNCH_ANIME();
bool IS_LEGAL (char _ch);
void LAUNCH_LOGIN();
void LAUNCH_REGISTER();
void PLAY_A();
void PLAY_B();
void PLAY_C();
bool CMP_A(USER _X, USER _Y);
bool CMP_B(USER _X, USER _Y);
bool CMP_C(USER _X, USER _Y);
bool CMP_TPP(USER _X, USER _Y);
void RANK_SORT();
void PLAY_RANK();
void LAUNCH();


/*将鼠标移动到X Y*/
void CUR_GO(short x, short y) {
    COORD coord = {y, x};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	return;
}

void CAL_TPP(int _k) {
    user[_k].tpp[0] = (DB)user[_k].max_score + 20.0 * user[_k].tpp[1] + 100.0 * user[_k].tpp[2];
    CUR_GO (1,0); printf("     TPP: %.2f", user[_k].tpp[0]);
    return;
}

DB CAL_RATE (int _x, int _y) {
    return _y == 0 ? 0.0 : (DB)_x / (DB)_y;
}

int CAL_MID (int _x) {
    return 40 - (_x >> 1);
}

DB NOW () {
    return (DB) (clock()) / CLOCKS_PER_SEC;
}

/*
设置背景色、前景色
    0 = 黑色       8 = 灰色
    1 = 蓝色       9 = 淡蓝色
    2 = 绿色       10 = 淡绿色
    3 = 浅绿色     11 = 淡浅绿色
    4 = 红色       12 = 淡红色
    5 = 紫色       13 = 淡紫色
    6 = 黄色       14 = 淡黄色
    7 = 白色       15 = 亮白色
*/
void SET_COLOR(int _bcolor, int _foclor) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)(_bcolor << 4 | _foclor));
}

/*释放按键积压*/
void RELAX () {
    while (kbhit()) getch();
    return;
}

/*禁用快速编辑*/
void ANTI_GOD_OPT() {
    auto hStdin = ::GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    ::GetConsoleMode(hStdin, &mode);
    mode &= ~ENABLE_QUICK_EDIT_MODE;
    ::SetConsoleMode(hStdin, mode);
    return;
}

/*隐藏闪烁光标*/
void HIDE_CONSOLE_CURSOR() {
	CONSOLE_CURSOR_INFO cursor_info = {1, 0};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
	return;
}

/*更换标题*/
void SET_TITLE(LPCSTR lpTitle) {
	SetConsoleTitle(lpTitle);
	return;
}

/*程序初始化*/
void PROJECT_INIT() {
    system("chcp 65001"); //更换代码页，用于使用Consolas字体
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof cfi;
	cfi.nFont = 0;
	cfi.dwFontSize.X = 10;
	cfi.dwFontSize.Y = 20;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFO consoleCurrentFont;
	GetCurrentConsoleFont(handle, FALSE, &consoleCurrentFont);
    srand(time(0));
    ANTI_GOD_OPT();
	SET_TITLE("OneTap");
	system("mode con:cols=80 lines=30");
	system("color F5");
	system("CLS");
	HIDE_CONSOLE_CURSOR();
	return;
}

/*将某区域（输入主对角线端点）清空*/
void COVER_SPACE (short _X, short _Y, short __X, short __Y) {
    short _len = __Y - _Y + 1;
    for (register short i = _X; i <= __X; ++i) {
        CUR_GO (i, _Y);
        for (register short j = 1; j <= _len; ++j) printf(" ");
    }
    return;
}

void PRT_SCORE (DB _t, DB _rate) {
    // COVER_SPACE (1,72, 1, 79);
    CUR_GO (0, 61); printf ("Time use : %.2f ", _t);
    CUR_GO (1, 57); printf ("Correct rate : %.1f%% ", _rate * 100);
    return;
}

/*转场动画*/
void TRANS (int _T) {
    COVER_SPACE (3,0,29,79);
    SET_COLOR(15,4);
    CUR_GO (5,22); printf("   ____          _______          ");
    CUR_GO (6,22); printf("  / __ \\        |__   __|         ");
    SET_COLOR(15,12);
    CUR_GO (7,22); printf(" | |  | |_ __   ___| | __ _ _ __  ");
    CUR_GO (8,22); printf(" | |  | | '_ \\ / _ | |/ _` | '_ \\ ");
    SET_COLOR(15,9);
    CUR_GO (9,22); printf(" | |__| | | | |  __| | (_| | |_) |");
    CUR_GO (10,22); printf("  \\____/|_| |_|\\___|_|\\__,_| .__/ ");
    SET_COLOR(15,1);
    CUR_GO (11,22); printf("                           | |    ");
    CUR_GO (12,22); printf(" made by GeoTheR_211300089_|_|    ");
    SET_COLOR(15,4);
	FOR (i,1,40) {
        CUR_GO(20,0);
        FOR (j,1,i) printf("=");
        FOR (j,1,80 - (i << 1)) printf(" ");
        FOR (j,1,i) printf("=");
        Sleep(_T);
        CUR_GO(16,38);
        printf("%.0f %%",(double) (i / 40.0) * 100.0);
        if (i == 24) Sleep(rand()%_T + _T * 10);
        printf("\r");
    }
    SET_COLOR(15,5);
    RELAX ();
    return;
}

/*加载动画*/
void LAUNCH_ANIME() {
    TRANS (50);
    for (int i = 1; ; ++i) {
        CUR_GO(25,CAL_MID(25));
        printf("%s", (i & 1) ? "Press any key to enter...": "                         ");
        if (kbhit()) break;
        Sleep((i & 1) ? 800 : 600);
    }
    RELAX ();
    system("CLS");
	return;
}

/*判断字符是否为字母或数字*/
bool IS_LEGAL (char _ch) {
    if ((_ch >= '0' && _ch <= '9') ||
        (_ch >='a' && _ch <= 'z') ||
        (_ch >= 'A' && _ch <= 'Z')) return true;
    return false;
}

/*账号登陆*/
void LAUNCH_LOGIN() {
    COVER_SPACE(3,0,20,78);
    CUR_GO (12, 25); printf("Handle : ");
    CUR_GO (16, 23); printf("Password : ");
    int _len = 0;
    char _temp[100], _ch;
    bool _ok = false;
    while (!_ok) {
        while (true) {
            if (kbhit()) {
                _ch = getch();
                if (_ch == 13) break;
                if (_ch == 8) {
                    --_len; _len = max (_len, 0);
                    CUR_GO (12, 35 + _len); printf(" ");CUR_GO(12, 35 + _len);
                } else if (IS_LEGAL(_ch)) {
                    _temp[_len++] = _ch;
                    CUR_GO (12, 34 + _len); printf("%c",_ch);
                }
            }
            Sleep(25);
        }
        bool __ok = true;
        FOR (i,1,user_cnt) {
            if (_len != strlen(user[i].name)) continue;
            __ok = true;
            FOR (j,0,strlen(user[i].name) - 1) {
                if (_temp[j] != user[i].name[j]) {
                    __ok = false;
                    break;
                }
            }
            if (__ok) {
                k = i;
                _ok = true;
                break;
            }
        }
        if (!_ok) {
            CUR_GO(10, 25);
            SET_COLOR (15, 4);
            printf("This user is not registered");
            SET_COLOR (15, 5);
            COVER_SPACE (12, 34, 12, 35 + _len);
        }
        _len = 0;
    }
    COVER_SPACE (10, 25, 10, 70);
    SET_COLOR (15, 2);
    CUR_GO(10, 25); printf("Please enter the password");
    SET_COLOR (15, 5);
    _len = 0; _ok = false;
    while (!_ok) {
        while (true) {
            if (kbhit()) {
                _ch = getch();
                if (_ch == 13) break;
                if (_ch == 8) {
                    --_len; _len = max (_len, 0);
                    CUR_GO (16, 35 + _len); printf(" ");CUR_GO(16, 35 + _len);
                } else if (IS_LEGAL(_ch)) {
                    _temp[_len++] = _ch;
                    CUR_GO (16, 34 + _len); printf("*");
                }
            }
            Sleep(25);
        }
        bool __ok = true;
        FOR (i,1,user_cnt) {
            if (_len != strlen(user[i].password)) continue;
            __ok = true;
            FOR (j,0,strlen(user[i].password) - 1) {
                if (_temp[j] != user[i].password[j]) {
                    __ok = false;
                    break;
                }
            }
            if (__ok) {
                k = i;
                _ok = true;
                break;
            }
        }
        if (!_ok) {
            CUR_GO(10, 25);
            SET_COLOR (15, 4);
            printf("This password is incorrect");
            SET_COLOR (15, 5);
            COVER_SPACE (16, 34, 16, 35 + _len);
        }
        _len = 0;
    }
    COVER_SPACE (10, 25, 10, 70);
    return;
}

/*注册并登陆*/
void LAUNCH_REGISTER() {
    COVER_SPACE(3,0,20,78);
    SET_COLOR (15, 2);
    CUR_GO (8, 12); printf("The user name or password consists of letters and numbers");
    CUR_GO (9, 24); printf("and cannot exceed 18 characters");
    CUR_GO (10, CAL_MID(49)); printf("The password shouldn't be less than 10 characters");
    CUR_GO (11, CAL_MID(45)); printf("and must consists of both letters and numbers");
    SET_COLOR (15, 5);
    CUR_GO (12, 25); printf("Handle : ");
    CUR_GO (16, 23); printf("Password : ");
    int _len = 0;
    char _temp[100], _ch;
    bool _ok = false;
    while (!_ok) {
        while (true) {
            if (kbhit()) {
                _ch = getch();
                if (_ch == 13) break;
                if (_ch == 8) {
                    --_len; _len = max (_len, 0);
                    CUR_GO (12, 35 + _len); printf(" ");CUR_GO(12, 35 + _len);
                } else if (IS_LEGAL(_ch)) {
                    _temp[_len++] = _ch;
                    CUR_GO (12, 34 + _len); printf("%c",_ch);
                }
            }
            Sleep(25);
        }
        int _ck_same = 1;
        FOR (_, 1, user_cnt) {
            if (_len != strlen(user[_].name)) continue;
            int __ck = 1;
            FOR (__, 0, _len - 1) if (user[_].name[__] != _temp[__]) {
                __ck = 0;
                break;
            }
            if (__ck) _ck_same = 0;
        }
        if (_len <= 18 && _ck_same) {
            _ok = true;
            k = user_cnt + 1;
            FOR (j,0,_len - 1) user[k].name[j] = _temp[j];
        }
        // cout << _len <<" "<<_ck_same<<" "<< (_w & _l)<<endl;
        if (!_ok) {
            SET_COLOR(15, 5);
            COVER_SPACE (8,0,11,79);
            SET_COLOR (15, 4);
            CUR_GO (8, 12); printf("The user name or password consists of letters and numbers");
            CUR_GO (9, 24); printf("and cannot exceed 18 characters");
            CUR_GO (10, CAL_MID(49)); printf("The password shouldn't be less than 10 characters");
            CUR_GO (11, CAL_MID(45)); printf("and must consists of both letters and numbers");
            if (!_ck_same) {CUR_GO (17, CAL_MID(51)); printf("              This name has been used              ");}
            SET_COLOR (15, 5);
            COVER_SPACE (12, 34, 12, 35 + _len);
        }
        _len = 0;
    }
    COVER_SPACE (8,0,11,79);
    SET_COLOR (15, 2);
    CUR_GO (8, 12); printf("The user name or password consists of letters and numbers");
    CUR_GO (9, 24); printf("and cannot exceed 18 characters");
    CUR_GO (10, CAL_MID(49)); printf("The password shouldn't be less than 10 characters");
    CUR_GO (11, CAL_MID(45)); printf("and must consists of both letters and numbers");
    CUR_GO (17, CAL_MID(53)); printf("              Please enter the password              ");
    CUR_GO (18,14); printf("Press the \'/\' key to view the password for one second");
    SET_COLOR (15, 5);
    _len = 0; _ok = false;
    while (!_ok) {
        while (true) {
            if (kbhit()) {
                _ch = getch();
                if (_ch == 13) break;
                if (_ch == 8) {
                    --_len; _len = max (_len, 0);
                    CUR_GO (16, 35 + _len); printf(" ");CUR_GO(16, 35 + _len);
                }  else if (_ch == '/') {
                    CUR_GO (16, 35); FOR (j,0,_len - 1) printf("%c",_temp[j]);
                    Sleep(1000);
                    CUR_GO (16, 35); FOR (j,0,_len - 1) printf("*");
                    RELAX ();
                } else if (IS_LEGAL(_ch)) {
                    _temp[_len++] = _ch;
                    CUR_GO (16, 34 + _len); printf("*");
                }
            }
            Sleep(25);
        }
        int _w = 0, _l = 0;
        FOR (_, 0, _len - 1) {
            if (_temp[_] >= '0' && _temp[_] <= '9') _w = 1;
            else _l = 1;
        }

        if (_len <= 18 && _len >= 10 && (_w & _l)) {
            _ok = true;
            ++user_cnt;
            FOR (j,0,_len - 1) user[k].password[j] = _temp[j];
        }
        if (!_ok) {
            COVER_SPACE (8,0,11,79);
            SET_COLOR (15, 4);
            CUR_GO (8, 12); printf("The user name or password consists of letters and numbers");
            CUR_GO (9, 24); printf("and cannot exceed 18 characters");
            CUR_GO (10, CAL_MID(49)); printf("The password shouldn't be less than 10 characters");
            CUR_GO (11, CAL_MID(45)); printf("and must consists of both letters and numbers");
            SET_COLOR (15, 5);
            COVER_SPACE (16, 34, 16, 35 + _len);
        }
        _len = 0;
    }
    // COVER_SPACE (10, 25, 10, 70);
    return;
}

void PLAY_A () {
    DB _t = 0.0, _rate = 0.0, _last;
    int _cnt = 0, ac_cnt = 0;
    char _temp[15];
    TRANS (5);
    COVER_SPACE (3,0,29,79);
    SET_COLOR (15, 2);
    CUR_GO (10,CAL_MID(58)); printf("The 30 random characters will be divided into three groups");
    CUR_GO (12, CAL_MID(46)); printf("For each group, you has 1.5 seconds to prepare");
    CUR_GO (14, CAL_MID(41)); printf("The faster, the more accurate, the better");
    SET_COLOR (15, 5);
    for (int i = 1; ; ++i) {
        CUR_GO(25,CAL_MID(25));
        printf("%s", (i & 1) ? "Press any key to enter...": "                         ");
        if (kbhit()) break;
        Sleep((i & 1) ? 800 : 600);
    }
    RELAX ();
    COVER_SPACE (3,0,29,79);
    FOR (_i,1,3) {
        PRT_SCORE (_t, _rate);
        int __cnt = 0;
        COVER_SPACE (3,0,29,79);
        FOR (i,1,10) _temp[i] = kbd[rand() % 66];
        FOR (_j,1,3) {
            CUR_GO (15,39); printf("%d",4 - _j);
            Sleep(500);
        }
        RELAX ();
        CUR_GO (15,39); printf(" ");
        SET_COLOR (14, 5);
        FOR (_j, 1, 10) {
            CUR_GO (13, 4 + _j * 6); printf ("%c", _temp[_j]);
            CUR_GO (15, 4 + _j * 6); printf (" ");
        }
        SET_COLOR (15, 5);
        _last = NOW ();
        while (true) {
            if (kbhit()) {
                int _ch = getch();
                if (_ch < 21 || _ch > 126) continue;
                ++_cnt; ++__cnt;
                CUR_GO (15, 4 + __cnt * 6);
                SET_COLOR (12, 5);
                if (_ch == _temp[__cnt]) {
                    SET_COLOR (10, 5);
                    ++ac_cnt;
                }
                printf ("%c",_ch);
                SET_COLOR (15, 5);
                _rate = CAL_RATE (ac_cnt, _cnt);
                if (__cnt == 10) break;
            }
            _t += NOW() - _last;
            _last = NOW();
            PRT_SCORE (_t, _rate);
            Sleep (25);
        }
    }
    TRANS(10);
    SET_COLOR(15, 5);
    COVER_SPACE (0,57,1,79);
    COVER_SPACE (3,0,29,79);
    SET_COLOR(15, 0);
    user[k].min_time[1] = min (_t, user[k].min_time[1]);
    user[k].max_rate[1] = max (_rate, user[k].max_rate[1]);
    user[k].tpp[1] = max (_rate * 6000.0 / (DB)_t, user[k].tpp[1]);
    CAL_TPP (k);
    CUR_GO (11, 31); printf("This time");
    CUR_GO (11, 51); printf("Best");
    CUR_GO (12, 19); printf("Time use : ");
    CUR_GO (13, 15); printf("Correct rate : ");
    CUR_GO (14, 18); printf("Score : ");
    SET_COLOR(15, 5);
    CUR_GO (12, 31); printf("%.2f",_t);
    CUR_GO (13, 31); printf("%.1f%%",_rate * 100.0);
    CUR_GO (14, 31); printf("%.2f",_rate * 6000.0 / (DB)_t);

    CUR_GO (12, 51); printf("%.2f",user[k].min_time[1]);
    CUR_GO (13, 51); printf("%.1f%%",user[k].max_rate[1] * 100.0);
    CUR_GO (14, 51); printf("%.2f",user[k].tpp[1]);

    Sleep(1500); RELAX ();
    CUR_GO (20, CAL_MID(11)); printf ("1 :: Remake");
    SET_COLOR(15, 4);
    CUR_GO (22, CAL_MID(23)); printf ("ESC :: Back to the hall");
    SET_COLOR(15, 5);
    while (true) {
        char _ch = getch();
        if (_ch == '1') {
            HALL (1);
            return;
        } else if (_ch = 0x1B) {
            HALL (0);
            return;
        }
        Sleep(25);
    }
    return;
}

void PLAY_B () {
    DB _t = 0.0, _rate = 0.0, _last;
    int _cnt = 0, ac_cnt = 0;
    int _f[5], __f;
    __f = rand() % text_cnt + 1;
    _f[1] = __f;
    while (__f == _f[1]) __f = rand() % text_cnt + 1; _f[2] = __f;
    while (__f == _f[1] || __f == _f[2]) __f = rand() % text_cnt + 1; _f[3] = __f;
    TRANS (5);
    COVER_SPACE (3,0,29,79);
    SET_COLOR (15, 2);
    CUR_GO (10,CAL_MID(23)); printf("You will get an article");
    CUR_GO (12, CAL_MID(10)); printf("Just do it");
    CUR_GO (14, CAL_MID(41)); printf("The faster, the more accurate, the better");
    SET_COLOR (15, 5);
    for (int i = 1; ; ++i) {
        CUR_GO(25,CAL_MID(25));
        printf("%s", (i & 1) ? "Press any key to enter...": "                         ");
        if (kbhit()) break;
        Sleep((i & 1) ? 800 : 600);
    }
    RELAX ();
    COVER_SPACE (3,0,29,79);
    FOR (_j,1,3) {
        CUR_GO (15,39); printf("%d",4 - _j);
        Sleep(500);
    }
    RELAX ();
    CUR_GO (15,39); printf(" ");

    PRT_SCORE (_t, _rate);
    COVER_SPACE (3,0,29,79);

    SET_COLOR (14, 5);
    FOR (i,1,3) {
        CUR_GO (9 + i * 3, CAL_MID(txt[_f[i]].len)); printf("%s",txt[_f[i]].text);
        CUR_GO (10 + i * 3, CAL_MID(txt[_f[i]].len)); FOR (j,1,txt[_f[i]].len) printf(" ");
    }
    SET_COLOR (15, 5);

    _last = NOW ();
    FOR (i,1,3) {
        int __cnt = 0;
        while (true) {
            if (kbhit()) {
                int _ch = getch();
                if (_ch < 21 || _ch > 126) continue;
                ++_cnt; ++__cnt;
                CUR_GO (10 + i * 3, CAL_MID(txt[_f[i]].len) + __cnt - 1);
                SET_COLOR (12, 5);
                if (_ch == txt[_f[i]].text[__cnt - 1]) {
                    SET_COLOR (10, 5);
                    ++ac_cnt;
                }
                printf ("%c",_ch);
                SET_COLOR (15, 5);
                _rate = CAL_RATE (ac_cnt, _cnt);
                if (__cnt == txt[_f[i]].len) break;
            }
            _t += NOW() - _last;
            _last = NOW();
            PRT_SCORE (_t, _rate);
            Sleep (25);
        }
    }

    TRANS(10);
    SET_COLOR(15, 5);
    COVER_SPACE (0,57,1,79);
    COVER_SPACE (3,0,29,79);
    SET_COLOR(15, 0);
    user[k].min_time[2] = min (_t, user[k].min_time[2]);
    user[k].max_rate[2] = max (_rate, user[k].max_rate[2]);
    user[k].tpp[2] = max (_rate * 6000.0 / (DB)_t, user[k].tpp[2]);
    CAL_TPP (k);
    CUR_GO (11, 31); printf("This time");
    CUR_GO (11, 51); printf("Best");
    CUR_GO (12, 19); printf("Time use : ");
    CUR_GO (13, 15); printf("Correct rate : ");
    CUR_GO (14, 18); printf("Score : ");
    SET_COLOR(15, 5);
    CUR_GO (12, 31); printf("%.2f",_t);
    CUR_GO (13, 31); printf("%.1f%%",_rate * 100.0);
    CUR_GO (14, 31); printf("%.2f",_rate * 6000.0 / (DB)_t);

    CUR_GO (12, 51); printf("%.2f",user[k].min_time[2]);
    CUR_GO (13, 51); printf("%.1f%%",user[k].max_rate[2] * 100.0);
    CUR_GO (14, 51); printf("%.2f",user[k].tpp[2]);

    Sleep(1500); RELAX ();
    CUR_GO (20, CAL_MID(11)); printf ("1 :: Remake");
    SET_COLOR(15, 4);
    CUR_GO (22, CAL_MID(23)); printf ("ESC :: Back to the hall");
    SET_COLOR(15, 5);
    while (true) {
        char _ch = getch();
        if (_ch == '1') {
            HALL (2);
            return;
        } else if (_ch = 0x1B) {
            HALL (0);
            return;
        }
        Sleep(25);
    }
    return;
}

int ABS (int _x) {
    return _x < 0 ? -_x : _x;
}

void PLAY_C () {
    int _note[6], _notey[6];
    FOR (i,0,65) vis[i] = 0;
    int _cnt = 0, ac_cnt = 0;
    TRANS (10);
    COVER_SPACE (3,0,29,79);
    SET_COLOR (15, 2);
    CUR_GO (10,CAL_MID(40)); printf("Two young girls explore a keyboard world");
    CUR_GO (12, CAL_MID(44)); printf("filled with sound: a past to be uncovered...");
    CUR_GO (14, CAL_MID(62)); printf("Each awakens in this blank, ruin-dotted world to discover that");
    CUR_GO (16, CAL_MID(62)); printf("she is equally blank, remembering nothing of what came before");
    CUR_GO (18, CAL_MID(44)); printf("Please hit the key when the light is passing");
    CUR_GO (20, CAL_MID(43)); printf("Help girls recover their lost typing memory");
    SET_COLOR (15, 5);
    for (int i = 1; ; ++i) {
        CUR_GO(25,CAL_MID(25));
        printf("%s", (i & 1) ? "Press any key to enter...": "                         ");
        if (kbhit()) break;
        Sleep((i & 1) ? 800 : 600);
    }
    RELAX ();
    COVER_SPACE (3,0,29,79);
    FOR (_j,1,3) {
        CUR_GO (15,39); printf("%d",4 - _j);
        Sleep(500);
    }
    RELAX ();
    CUR_GO (15,39); printf(" ");

    COVER_SPACE (3,0,29,79);

    SET_COLOR (15, 5);

    FOR (_i, 1, 10) {
        int _hd = 62;
        while (_hd == 62 || _hd == 55 || _hd == 56 || _hd == 61) _hd = 51 + rand() % 15;
        FOR (__i, 0, 2) _note[__i + 1] = __i * 17 + rand() % 17; _note[4] = _hd;
        // vis[rand() % 17] = vis[17 + rand() % 17] = vis[34 + rand() % 17] = vis[_hd] = _i;

        DB _k = 50.0 / (DB) (pos_y[_note[4]] - pos_y[_note[1]]);
        _notey[1] = 15;
        _notey[2] = _notey[1] + (int)((DB)(pos_y[_note[2]] - pos_y[_note[1]]) * _k);
        _notey[3] = _notey[1] + (int)((DB)(pos_y[_note[3]] - pos_y[_note[1]]) * _k);
        _notey[4] = _notey[1] + (int)((DB)(pos_y[_note[4]] - pos_y[_note[1]]) * _k);


        int _d = (_i & 1) ? 1 : -1;
        FOR (__i,1,4) {
            CUR_GO (pos_x[_note[__i]], _notey[__i]);
            printf("%c",kbd[_note[__i]]);
        }
        RELAX ();
        if (_i & 1) for (int _j = 1; _j <= 79; _j += _d) {
            SET_COLOR (15, 9);
            FOR (__i, 3, 29) CUR_GO(__i, _j - 1), printf(" |");
            SET_COLOR (15, 5);
            Sleep (50);
            if (kbhit()) {
                char _ch = getch ();
                FOR (_c, 1, 4) if (_ch == kbd[_note[_c]] && ABS(_notey[_c] - _j) <= 2 && vis[_note[_c]] != _i) {
                    ac_cnt += 250;
                    SET_COLOR (15, 2);
                    CUR_GO (1,CAL_MID(10)); printf("   PURE   ");
                    SET_COLOR (15, 5);
                    vis[_note[_c]] = _i;
                } else if (_ch == kbd[_note[_c]] && ABS(_notey[_c] - _j) <= 4 && vis[_note[_c]] != _i) {
                    ac_cnt += 125;
                    SET_COLOR (15, 4);
                    CUR_GO (1,CAL_MID(9)); printf("   FAR   ");
                    SET_COLOR (15, 5);
                    vis[_note[_c]] = _i;
                }
                CUR_GO(1, 70); printf("%d",ac_cnt);
            }
            RELAX();
        } else for (int _j = 78; _j >= 0; _j += _d) {
            SET_COLOR (15, 9);
            FOR (__i, 3, 29) CUR_GO(__i, _j), printf("| ");
            SET_COLOR (15, 5);
            Sleep (50);
            if (kbhit()) {
                char _ch = getch ();
                FOR (_c, 1, 4) if (_ch == kbd[_note[_c]] && ABS(_notey[_c] - _j) <= 2 && vis[_note[_c]] != _i) {
                    ac_cnt += 250;
                    SET_COLOR (15, 2);
                    CUR_GO (1,CAL_MID(10)); printf("   PURE   ");
                    SET_COLOR (15, 5);
                    vis[_note[_c]] = _i;
                } else if (_ch == kbd[_note[_c]] && ABS(_notey[_c] - _j) <= 4 && vis[_note[_c]] != _i) {
                    ac_cnt += 125;
                    SET_COLOR (15, 4);
                    CUR_GO (1,CAL_MID(9)); printf("   FAR   ");
                    SET_COLOR (15, 5);
                    vis[_note[_c]] = _i;
                }
                CUR_GO(1, 70); printf("%d",ac_cnt);
            }
            RELAX();
        }
    }
    TRANS(10);
    SET_COLOR(15, 5);
    COVER_SPACE (1, 20, 1, 79);
    COVER_SPACE (3,0,29,79);
    SET_COLOR(15, 0);
    CUR_GO (11, 31); printf("This time");
    CUR_GO (11, 51); printf("Best");
    // CUR_GO (12, 19); printf("Time use : ");
    CUR_GO (13, 15); printf("Score : ");
    SET_COLOR(15, 5);
    // CUR_GO (12, 31); printf("%.2f",_t);
    CUR_GO (13, 31); printf("%d",ac_cnt);
    // CUR_GO (12, 51); printf("%.2f",user[k].min_time[2])
    user[k].max_score = max (user[k].max_score, ac_cnt);
    CAL_TPP (k);
    CUR_GO (13, 51); printf("%d",user[k].max_score);
    Sleep(1500); RELAX ();
    CUR_GO (20, CAL_MID(11)); printf ("1 :: Remake");
    SET_COLOR(15, 4);
    CUR_GO (22, CAL_MID(23)); printf ("ESC :: Back to the hall");
    SET_COLOR(15, 5);
    while (true) {
        char _ch = getch();
        if (_ch == '1') {
            HALL (3);
            return;
        } else if (_ch = 0x1B) {
            HALL (0);
            return;
        }
        Sleep(25);
    }
    return;
}

bool CMP_A(USER _X, USER _Y) {return _X.tpp[1] > _Y.tpp[1];}
bool CMP_B(USER _X, USER _Y) {return _X.tpp[2] > _Y.tpp[2];}
bool CMP_C(USER _X, USER _Y) {return _X.max_score > _Y.max_score;}
bool CMP_TPP(USER _X, USER _Y) {return _X.tpp[0] > _Y.tpp[0];}

void RANK_SORT () {
    USER usr[50];
    FOR (i,1,user_cnt) {
        user[i].tpp[0] = (DB)user[i].max_score + 20.0 * user[i].tpp[1] + 100.0 * user[i].tpp[2];
        usr[i] = user[i];
    }
    SET_COLOR (15, 0);
    CUR_GO (5, 1); printf("WesternFastGunners");
    CUR_GO (5, 21);printf("KeyboardPianist");
    CUR_GO (5, 41);printf("Arcytus");
    CUR_GO (5, 61);printf("TPP");
    SET_COLOR (15, 5);
    sort (usr + 1, usr + user_cnt + 1, CMP_A);
    FOR (i,1,min (5, user_cnt)) {
        CUR_GO (7 + (i - 1) * 3, 1); printf("%d :: %.2f",i,usr[i].tpp[1]);
        CUR_GO (8 + (i - 1) * 3, 1); printf("%s",usr[i].name);
    }

    sort (usr + 1, usr + user_cnt + 1, CMP_B);
    FOR (i,1,min (5, user_cnt)) {
        CUR_GO (7 + (i - 1) * 3, 21); printf("%d :: %.2f",i,usr[i].tpp[2]);
        CUR_GO (8 + (i - 1) * 3, 21); printf("%s",usr[i].name);
    }
    sort (usr + 1, usr + user_cnt + 1, CMP_C);
    FOR (i,1,min (5, user_cnt)) {
        CUR_GO (7 + (i - 1) * 3, 41); printf("%d :: %d",i,usr[i].max_score);
        CUR_GO (8 + (i - 1) * 3, 41); printf("%s",usr[i].name);
    }
    sort (usr + 1, usr + user_cnt + 1, CMP_TPP);
    FOR (i,1,min (5, user_cnt)) {
        CUR_GO (7 + (i - 1) * 3, 61); printf("%d :: %.2f",i,usr[i].tpp[0]);
        CUR_GO (8 + (i - 1) * 3, 61); printf("%s",usr[i].name);
    }
    return;
}

void PLAY_RANK () {
    TRANS(5);
    COVER_SPACE (3,0,29,79);
    RANK_SORT ();
    SET_COLOR(15, 2);
    CUR_GO (25, CAL_MID(19)); printf ("TPP is TaPPotential");
    CUR_GO (26, CAL_MID(47)); printf ("which is the evaluation value of typing ability");
    SET_COLOR(15, 5);
    Sleep(1000);
    SET_COLOR(15, 4);
    CUR_GO (27, CAL_MID(23)); printf ("ESC :: Back to the hall");
    SET_COLOR(15, 5);
    while (true) {
        char _ch = getch();
        if (_ch = 0x1B) {
            HALL (0);
            return;
        }
        Sleep(50);
    }
    return;
}

/*大厅*/
void HALL (int _k) {
    TRANS (10);
    CAL_TPP(k);
    COVER_SPACE (3,0,29,79);
    if (_k) {
        if (_k == 1) PLAY_A ();
        else if (_k == 2) PLAY_B ();
        else if (_k == 3) PLAY_C ();
    }
    CUR_GO (0,0); printf(" Welcome: %s", user[k].name);
    CUR_GO (1,0); printf("     TPP: %.2f    ", user[k].tpp[0]);
    CUR_GO (2,0); FOR (i,1,80) printf("~");
    SET_COLOR (15,2);
    CUR_GO (8, 28); printf("1 :: Western fast gunners");
    CUR_GO (10, 28); printf("2 :: Keyboard pianist");
    CUR_GO (12, 28); printf("3 :: Arcytus");
    SET_COLOR (15, 5);
    CUR_GO (14, 28); printf("r :: Rankings");
    SET_COLOR (15, 4);
    CUR_GO (16, 26); printf("ESC :: Quit OneTap");
    SET_COLOR (15, 5);
    while (true) {
        if (kbhit()) {
            char _ch = getch();
            if (_ch == '1') {PLAY_A(); break;}
            else if (_ch == '2') {PLAY_B(); break;}
            else if (_ch == '3') {PLAY_C(); break;}
            else if (_ch == 'r') {PLAY_RANK(); break;}
            else if (_ch == 0x1B) break;
        }
        Sleep (25);
    }
    system("CLS");
    freopen ("./DATE/user_date.txt","w",stdout);
    printf("%d\n",user_cnt);
    FOR (i,1,user_cnt) {
        printf("%s ",user[i].name); printf("%s ",user[i].password);
        FOR (j,1,2) printf("%.3f ",user[i].min_time[j]), printf("%.3f ",user[i].max_rate[j]);
        printf("%d",user[i].max_score);
        FOR (j,0,2) printf(" %.3f",user[i].tpp[j]);
        printf("\n");
    }
    fclose(stdout);
    // TRANS(10);
    return;
}

/*登陆界面*/
void LAUNCH() {
    system("CLS");
    freopen ("./DATE/user_date.txt","r",stdin);
    scanf("%d",&user_cnt);
    FOR (i,1,user_cnt) {
        scanf("%s",user[i].name); scanf("%s",user[i].password);
        FOR (j,1,2) scanf("%lf",&user[i].min_time[j]), scanf("%lf",&user[i].max_rate[j]);
        scanf("%d",&user[i].max_score);
        FOR (i,0,2) scanf("%lf",&user[i].tpp[i]);
    }
    fclose(stdin);
    freopen ("./DATE/text_date.txt","r",stdin);
    char _X[10];
    gets (_X);
    int __X = 0;
    for (int i = strlen(_X) - 1; i >= 0; --i) __X = __X * 10 + _X[i] - '0';
    text_cnt = __X;
    FOR (i,1,text_cnt) gets(txt[i].text), txt[i].len = strlen (txt[i].text);
    fclose(stdin);
    RELAX();
    CUR_GO (10, 28); printf("1 :: Login into OneTap");
    CUR_GO (16, 28); printf("2 :: Register in OneTap");
    while (true) {
        if (kbhit()) {
            char ch = getch();
            if (ch == '1') {LAUNCH_LOGIN(); break;}
            else if (ch == '2') {LAUNCH_REGISTER(); break;}
        }
        Sleep(25);
    }
    HALL(0);
    return;
}

int main() {
    PROJECT_INIT();
    LAUNCH_ANIME();
    LAUNCH();
    return 0;
}