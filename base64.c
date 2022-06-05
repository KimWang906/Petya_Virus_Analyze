/* Base64 encoder/decoder. Originally Apache file ap_base64.c */

#include <string.h>
/*
    strcpy(str1, str2)를 호출할 때 strcpy는 str2가 가리키는 문자열이 str1이 가리키는 배열의 크기에 맞는지 판단을 해줄 수 없다. 
    만약 str1이 크기가 n인 배열을 가리킨다고 하자. 이때 str2가 크키가 최대 n - 1인 문자열이라면 복사가 가능할 것이다. 
    하지만 만약 str2이 이보다 더 큰 문자열이라면 정의되지 않은 행동을 야기할 것이다
    (strcpy1는 언제나 첫번째 널문자를 만나기 전까지 계속 복사하기 때문에 str1이 가리키는 배열을 벗어나서도 복사를 할 것이다)

    문자열을 복사할 땐 strncpy 함수를 호출하는게 더 느리더라도 더 안전하다. 
    strcnpy는 strcpy와 거의 같지만 세번째 입력변수로 복사할 문자의 개수를 제한한다는 점에서 차이점이 있다. 
    str2를 str1에 복사하려면 다음과 같이 strncpy를 호출해주어야한다.

    strncpy(str1, str2, sizeof(str1));
    str1이 str2에 저장된 문자열(널 문자까지 포함해서)을 받을만큼 충분히 큰 이상 복사는 정상적으로 작동할 것이다. 
    물론 그렇다고 해서 strncpy도 주의할 점이 없는 건 아니고 한가지 문제가 있다. 
    만약 str2에 저장된 문자열의 크기가 str1 배열의 크기보다 크거나 같으면 str1을 널 문자로 끝내주지 않는다. 
    그렇기 때문에 strncpy를 안전하게 사용하려면 다음과 같이 사용하면 된다.

    strncpy(str1, str2, sizeof(str1) - 1);
    str1[sizeof(str1) - 1] = '\0';
    두번째 구문을 통해 strncpy가 str2의 널 문자를 str1에 복사해주지 못했더라도 str1은 언제나 널문자로 끝남을 보장할 수 있다.

    strlen (문자열 크기) 함수
    strlen 함수의 원형은 다음과 같다.

    size_t strlen(const char* str);
    C 라이브러리에서 typedef로 정의된 size_t2는 C의 미부호 정수형 중 하나를 나타내는 형이름이다. 
    우리가 극단적으로 긴 문자열을 다루지 않는 한 size_t와 같은 기술적인 내용은 중요하지 않다. 
    즉, strlen의 반환값을 정수 취급을 해줄 수도 있을 것이다.

    strlen은 문자열 str의 길이를 반환해준다. 
    즉, 첫 널 문자를 제외한 str의 문자의 개수다.
    ex )
        int len;

        len = strlen("abc"); len의 값: 3
        len = strlen("");    len의 값: 0
        strcpy(str1, "abc");
        len = strlen(str1);  len의 값: 3
    위의 예제에선 아주 중요한 내용을 포함하고 있다. 
    배열이 입력변수로서 주어지면 strlen은 배열 자체의 길이를 측정하지 않고 배열의 저장된 문자열의 길이를 반환한다.

    strcat (문자열 연결) 함수
    strcat 함수의 원형은 다음과 같다.

    char* strcat(char* destination, const char* source);
    strcat은 문자열 source의 내용을 문자열 destination 끝에 이어붙여준다.
    그러고 destination(결과 문자열의 포인터)을 반환한다.

    다음은 strcat을 사용한 예시이다:

        strcpy(str1, "abc");
        strcat(str1, "def"); str1: "abcdef"
        strcpy(str1, "abc");
        strcpy(str2, "def");
        strcat(str1, str2); str1: "abcdef"

    strcpy와 마찬가지로 strcat의 반환값은 보통 버려진다. 다음 예시에서는 해당 반환값을 이용하는 예시이다:

        strcpy(str1, "abc");
        strcpy(str2, "def");
        strcat(str1, strcat(str2, "ghi"));
            str: "abcdefghi"; str2: "defghi"

    strncat 함수3는 strcat보다 더 느리지만 더 안전한 함수이다.
    strncpy처럼 복사할 문자의 개수에 제한을 두는 세번째 입력변수를 받는다.
    strncat(str1, str2, sizeof(str1) - strlen(str1) - 1);
    strncat은 str1의 끝에 널 문자를 넣어줄 것이다. 
    이는 세번째 입력변수(복사할 문자의 개수)에 포함된 것이 아니다. 
    이 예시에선 세번째 입력변수는 str1에 남아있는 공간의 크기
    (표현식 sizeof(str1) - strlen(str1)의 값)를 계산한 값에 1을 빼서 널 문자를 위한 공간을 확보해주었다.

    strcmp (문자열 비교) 함수
    strcmp 함수의 원형은 다음과 같다.

    int strcmp(const char* str1, const char* str2);
    strcmp는 문자열 str1과 str2를 비교해 str1이 str2보다 작은지, 같은지, 큰지 여부에 따라 0보다 작거나, 같거나, 더 큰 값을 반환한다[Q & A]. 예를 들어 str1이 str2보다 작은지를 보려면 다음과 같이 작성할 수 있다:

        if (strcmp(str1, str2) < 0) { str1 < str2 인가?
            …
        str1이 str2보다 작거나 같은지를 보려면 다음과 같이 작성할 수 있다.

        if (strcmp(str1, str2) <= 0) {  str1 <= str2 인가?
            …
        알맞은 관계 연산자(<, <=, >, >=)나 동등 연산자(==, !=)를 골라 str1과 str2 간의 관계를 판별할 수 있다.

    strcmp는 문자열을 사전식 순서로 비교를 한다. 
    정확히 말하자면 strcmp는 다음 조건일 때 s1이 s2보다 작다고 판별한다.

    s1과 s2의 i번째 문자까지는 같지만, s1의 (i + 1)번째 문자가 s2의 (i + 1)번째 문자보다 작을 때
    예를 들자면 "abc"는 "bcd"보다 작고, "abd"는 "abe"보다 작다.
    s1의 모든 문자가 s2와 같지만, s1보다 s2가 더 크기가 클 경우. 예를 들어 "abc"는 "abcd"보다 작다.
    strcmp는 두 문자열의 문자들을 비교하기 때문에, 문자들이 해당하는 숫자 코드를 기반으로 사용하게 된다. 
    그러므로 문자 집합에 대한 배경지식이 있다면 strcmp의 기능을 이해하는데 편할 것이다. 다음은 ASCII 문자열 집합4의 몇가지 중요한 특성들이다

    A에서 Z, a에서 z, 0에서 9라는 연속된 문자들은 마찬가지로 연속된 코드를 갖는다.
    모든 대문자는 소문자보다 작다. (ASCII에서 65에서 90까지의 코드들이 대문자이며; 97과 122까지의 코드가 소문자다.)
    숫자는 문자보다 작다. (48에서 57까지가 숫자다.)
    띄어쓰기는 모든 출력 가능한 문자보다 작다. (띄어쓰기는 ASCII에서 32라는 값을 갖는다.)

출처 : https://wikidocs.net/86258
*/

#include "base64.h"

/* aaaack but it's fast and const should make it shared text page. */
static const unsigned char pr2six[256] =
{
    /* ASCII table */
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
    64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
    64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
    64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
};

int Base64decode_len(const char *bufcoded)
{
    int nbytesdecoded;
    register const unsigned char *bufin;
    register int nprbytes;

    bufin = (const unsigned char *) bufcoded;
    while (pr2six[*(bufin++)] <= 63);

    nprbytes = (bufin - (const unsigned char *) bufcoded) - 1;
    nbytesdecoded = ((nprbytes + 3) / 4) * 3;

    return nbytesdecoded + 1;
}

int Base64decode(char *bufplain, const char *bufcoded)
{
    int nbytesdecoded;
    register const unsigned char *bufin;
    register unsigned char *bufout;
    register int nprbytes;

    bufin = (const unsigned char *) bufcoded;
    while (pr2six[*(bufin++)] <= 63);
    nprbytes = (bufin - (const unsigned char *) bufcoded) - 1;
    nbytesdecoded = ((nprbytes + 3) / 4) * 3;

    bufout = (unsigned char *) bufplain;
    bufin = (const unsigned char *) bufcoded;

    while (nprbytes > 4) {
    *(bufout++) =
        (unsigned char) (pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4);
    *(bufout++) =
        (unsigned char) (pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2);
    *(bufout++) =
        (unsigned char) (pr2six[bufin[2]] << 6 | pr2six[bufin[3]]);
    bufin += 4;
    nprbytes -= 4;
    }

    /* Note: (nprbytes == 1) would be an error, so just ingore that case */
    if (nprbytes > 1) {
    *(bufout++) =
        (unsigned char) (pr2six[*bufin] << 2 | pr2six[bufin[1]] >> 4);
    }
    if (nprbytes > 2) {
    *(bufout++) =
        (unsigned char) (pr2six[bufin[1]] << 4 | pr2six[bufin[2]] >> 2);
    }
    if (nprbytes > 3) {
    *(bufout++) =
        (unsigned char) (pr2six[bufin[2]] << 6 | pr2six[bufin[3]]);
    }

    *(bufout++) = '\0';
    nbytesdecoded -= (4 - nprbytes) & 3;
    return nbytesdecoded;
}

static const char basis_64[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int Base64encode_len(int len)
{
    return ((len + 2) / 3 * 4) + 1;
}

int Base64encode(char *encoded, const char *string, int len)
{
    int i;
    char *p;

    p = encoded;
    for (i = 0; i < len - 2; i += 3) {
    *p++ = basis_64[(string[i] >> 2) & 0x3F];
    *p++ = basis_64[((string[i] & 0x3) << 4) |
                    ((int) (string[i + 1] & 0xF0) >> 4)];
    *p++ = basis_64[((string[i + 1] & 0xF) << 2) |
                    ((int) (string[i + 2] & 0xC0) >> 6)];
    *p++ = basis_64[string[i + 2] & 0x3F];
    }
    if (i < len) {
    *p++ = basis_64[(string[i] >> 2) & 0x3F];
    if (i == (len - 1)) {
        *p++ = basis_64[((string[i] & 0x3) << 4)];
        *p++ = '=';
    }
    else {
        *p++ = basis_64[((string[i] & 0x3) << 4) |
                        ((int) (string[i + 1] & 0xF0) >> 4)];
        *p++ = basis_64[((string[i + 1] & 0xF) << 2)];
    }
    *p++ = '=';
    }

    *p++ = '\0';
    return p - encoded;
}
