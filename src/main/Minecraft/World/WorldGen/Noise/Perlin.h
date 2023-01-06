#include <immintrin.h>
#include <vector>
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

class noise_2 {
public:


    const int P[512] = { 103,103,250,58,221,27,189,245,120,3,111,191,208,170,27,213,124,185,86,25,199,48,
    194,14,73,221,181,37,115,119,165,72,10,231,70,136,228,183,104,28,154,109,70,68,249,146,69,198,231,252,
    42,153,162,158,8,70,63,218,105,239,57,50,194,43,168,97,23,143,138,29,64,148,190,16,118,211,225,178,112,
    3,67,28,215,241,5,115,78,175,113,65,11,55,59,142,161,32,1,78,205,148,184,10,194,1,47,7,121,181,100,4,
    176,197,55,119,51,159,233,94,128,81,57,184,98,161,78,42,57,50,87,252,93,34,28,83,75,38,57,86,108,115,188,
    164,45,120,50,212,49,163,123,197,35,190,63,73,184,252,224,225,215,134,225,109,183,218,124,27,174,189,64,
    83,166,154,131,136,98,72,18,26,168,137,57,119,113,106,115,163,125,250,133,63,158,147,98,164,96,158,30,156,
    5,203,43,182,92,200,232,207,232,5,112,88,223,22,147,147,146,216,157,248,134,65,182,12,170,151,19,244,243,
    135,118,178,160,181,67,197,43,223,152,236,39,5,197,108,107,135,120,212,91,224,78,235,106,4,10,1,13,62,
        103,103,250,58,221,27,189,245,120,3,111,191,208,170,27,213,124,185,86,25,199,48,
    194,14,73,221,181,37,115,119,165,72,10,231,70,136,228,183,104,28,154,109,70,68,249,146,69,198,231,252,
    42,153,162,158,8,70,63,218,105,239,57,50,194,43,168,97,23,143,138,29,64,148,190,16,118,211,225,178,112,
    3,67,28,215,241,5,115,78,175,113,65,11,55,59,142,161,32,1,78,205,148,184,10,194,1,47,7,121,181,100,4,
    176,197,55,119,51,159,233,94,128,81,57,184,98,161,78,42,57,50,87,252,93,34,28,83,75,38,57,86,108,115,188,
    164,45,120,50,212,49,163,123,197,35,190,63,73,184,252,224,225,215,134,225,109,183,218,124,27,174,189,64,
    83,166,154,131,136,98,72,18,26,168,137,57,119,113,106,115,163,125,250,133,63,158,147,98,164,96,158,30,156,
    5,203,43,182,92,200,232,207,232,5,112,88,223,22,147,147,146,216,157,248,134,65,182,12,170,151,19,244,243,
    135,118,178,160,181,67,197,43,223,152,236,39,5,197,108,107,135,120,212,91,224,78,235,106,4,10,1,13,62 };

    std::vector<float> vecLookup;

    void init() {

        for (int i = 0; i < 512; i++) {
            if ((int)(P[i] & 3) == 0) {
                vecLookup.emplace_back(1.0f);
                vecLookup.emplace_back(1.0f);
            }
            if ((int)(P[i] & 3) == 1) {
                vecLookup.emplace_back(-1.0f);
                vecLookup.emplace_back(1.0f);
            }
            if ((int)(P[i] & 3) == 2) {
                vecLookup.emplace_back(-1.0f);
                vecLookup.emplace_back(-1.0f);
            }
            if ((int)(P[i] & 3) == 3) {
                vecLookup.emplace_back(1.0f);
                vecLookup.emplace_back(-1.0f);
            }

        }
    }

    fvec2 GetConstVec(int v) {
        int h = v & 3;
        if (h == 0)
            return fvec2(1.0, 1.0);
        else if (h == 1)
            return fvec2(-1.0, 1.0);
        else if (h == 2)
            return fvec2(-1.0, -1.0);
        else
            return fvec2(1.0, -1.0);
    }

    float DotProduct(vec2 a, vec2 b) {
        return (a.x * b.x) + (a.y * b.y);
    }

    float DotProduct(float ax, float ay, float bx, float by) {
        return (ax * bx) + (ay * by);
    }

    float* DotProduct4xAVX(vec2 a0, vec2 b0, vec2 a1, vec2 b1, vec2 a2, vec2 b2, vec2 a3, vec2 b3) {
        float results[4]{};
        __m256 a = _mm256_set_ps(a0.x, a0.y, a1.x, a1.y, a2.x, a2.y, a3.x, a3.y); //data1x8 = _mm256_load_ps(data_1);
        __m256 b = _mm256_set_ps(b0.x, b0.y, b1.x, b1.y, b2.x, b2.y, b3.x, b3.y); //data2x8 = _mm256_load_ps(data_2);
        __m256 result = _mm256_mul_ps(a, b);
        float* f = (float*)&result;
        __m128 a_ = _mm_set_ps(f[0], f[2], f[4], f[6]);
        __m128 b_ = _mm_set_ps(f[1], f[3], f[5], f[7]);
        __m128 result_ = _mm_mul_ps(a_, b_);
        return (float*)&result_;

    }



    float getNoise(float x, float y) {
        float xfreq = 1.1f;
        float yfreq = 1.1f;

        x *= xfreq;
        y *= yfreq;

        int fx = FastFloor(x);
        int fy = FastFloor(y);

        int X = fx & 0xFF;
        int Y = fy & 0xFF;

        float xf = x - (float)fx;
        float yf = y - (float)fy;

        vec2 TopRight(xf - 1.0f, yf - 1.0f);
        vec2 TopLeft(xf, yf - 1.0f);
        vec2 BottomRight(xf - 1.0f, yf);
        vec2 BottomLeft(xf, yf);

        float DotTopRight = DotProduct(TopRight, GetConstVec(P[P[X + 1] + Y + 1]));
        float DotTopLeft = DotProduct(TopLeft, GetConstVec(P[P[X] + Y + 1]));
        float DotBottomRight = DotProduct(BottomRight, GetConstVec(P[P[X + 1] + Y]));
        float DotBottomLeft = DotProduct(BottomLeft, GetConstVec(P[P[X] + Y]));

        float u = InterpQuintic(xf);
        float v = InterpQuintic(yf);

        return Lerp(Lerp(DotBottomLeft, DotTopLeft, v), Lerp(DotBottomRight, DotTopRight, v), u);

    }
private:
    __m256 FloatDatax8_Reg1 = _mm256_setr_ps(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    __m256 FloatDatax8_Reg2 = _mm256_setr_ps(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

    static float Lerp(float a, float b, float t) { return a + t * (b - a); }
    static float InterpQuintic(float t) { return t * t * t * (t * (t * 6 - 15) + 10); }
    template <typename NUM>
    static int FastFloor(NUM f) { return f >= 0 ? (int)f : (int)f - 1; }
};