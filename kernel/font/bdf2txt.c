/*
   bdf2txt
   BDFフォントの抽出ツール(主にはりぼて用)

   creator : boxnos
   license : KL-01

   usage   : bdf2txt < hoge.bdf > hoge.txt

   history : 2006-11-18 0.02 全面的に修正。
             2006-11-17 0.01 作ってみた。
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>

char *hexToBin(char *hex, char *buffer);

int main(void) {
    char buffer[256];
    char bin[256];

    FILE *in = stdin;
    FILE *out = stdout;

    while (fgets(buffer, sizeof(buffer), in)) {
        // BITMAP行まで飛ばす
        if (strncmp(buffer, "BITMAP", strlen("BITMAP")) != 0) {
            continue;
        }

        // BITMAPの次の行～ENDCHARの処理
        while (fgets(buffer, sizeof(buffer), in)) {
            if (strncmp(buffer, "ENDCHAR", strlen("ENDCHAR")) == 0) {
                putc('\n', out);
                break;
            }
            fputs(hexToBin(buffer, bin), out);
            //fputs(buffer, out);
            putc('\n', out);
        }

    }

    return 0;
}

char *hexToBin(char *hex, char *buffer) {
    static const char *maps[] = {
        "....", "...*", "..*.", "..**",
        ".*..", ".*.*", ".**.", ".***",
        "*...", "*..*", "*.*.", "*.**",
        "**..", "**.*", "***.", "****"
    };
    buffer[0] = '\0'; // bufferの初期化

    while (*hex && *hex != '\n' && *hex != '\r') {
        strcat(buffer, maps[(isdigit(*hex) ? *hex - '0' : tolower(*hex) - 'a' + 10)]);
        hex++;
    }
    return buffer;
}
