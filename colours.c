#include "colours.h"

struct colour get_random_color(void) {
    int red = rand() % 255;
    int blue = rand() % 255;
    int green = rand() % 255;

    struct colour col;
    col.r = red;
    col.g = green;
    col.b = blue;
    col.a = 255;
    return col;
}

SDL_Colour get_sdlcolour(struct colour col, int alpha) {
    struct colour c = get_rgb(col);
    return (SDL_Color){c.r, c.g, c.b, alpha};
}

double m_min(double arr[], int len) {
    double largest = arr[0];
    for (int i = 0; i < len; i ++) {
        if (arr[i] < largest) {
            largest = arr[i];
        }
    }
    return largest;
}

double m_max(double arr[], int len) {
    double largest = arr[0];
    for (int i = 0; i < len; i ++) {
        if (arr[i] > largest) {
            largest = arr[i];
        }
    }
    return largest;
}

bool m_equal(double a, double b) {
    return (a - b) * (a - b)  < 0.00001;
}

// https://en.wikipedia.org/wiki/HSL_and_HSV#From_RGB
// h = [0,360], s = [0,1], v = [0,1] 
// if s == 0, then h = -1 (undefined)
struct colour get_hs_l_v(struct colour c, enum colour_space sp) {
    struct colour ret = {};
    double r = (double)c.r / 255;
    double g = (double)c.g / 255;
    double b = (double)c.b / 255;

    double arr[] = {r, g, b};
    double max = m_max(arr, 3);
    double min = m_min(arr, 3);

    ret.v = max;

    double chroma = max - min;

    double lum = (max + min) / 2;
    ret.l = lum;

    if (m_equal(chroma, 0)) {
        ret.h = 0;
    } else if (m_equal(ret.v,r)) {
        ret.h = (g - b) / chroma;
    } else if (m_equal(max,g)) {
        ret.h = 2 + (b - r) / chroma;
    } else if (m_equal(max,b)) {
        ret.h = 4 + (r - g) / chroma;
    } else {
        printf("NOPE BAD ERROR\n");
    } 

    ret.h *= 60;
    ret.h = fmod(ret.h, 360);
    if (ret.h < 0) {
        ret.h += 360;
    }

    if (sp == CS_HSV) {
        if (m_equal(max,0)) {
            ret.s = 0;
        } else {
            ret.s = chroma / max;
        }
    } else {
        double arr[] = {ret.l, 1 - ret.l};
        if (m_equal(0, lum) || m_equal(1, lum)) {
            ret.s = 0;
        } else {
            ret.s = (ret.v - ret.l) / m_min(arr, 2); 
        }
    }
    ret.sp = CS_HSL;
    return ret;
}

struct colour get_hsl(struct colour c) {
    return get_hs_l_v(c, CS_HSL); 
}

struct colour get_hsv(struct colour c) {
    return get_hs_l_v(c, CS_HSV); 
}

double magic_hsv_function(int n, struct colour c) {
    double res = 0;
    double k = fmod(n + (c.h / 60), 6);
    double arr[] = {k, 4 - k, 1};
    double k_b = m_min(arr, 3);
    double k_c = 0 > k_b ? 0 : k_b;
    res = c.v - c.v * c.s * k_c; 
    return res;
}

struct colour get_rgb_from_hsv(struct colour c) {

    double r = magic_hsv_function(5, c);
    double g = magic_hsv_function(3, c);
    double b = magic_hsv_function(1, c);

    struct colour res;
    res.r = round(r * 255);
    res.g = round(g * 255);
    res.b = round(b * 255);
    return res;

}

double magic_hsl_function(int n, struct colour c) {
    double a = c.s * ( c.l < (1-c.l) ? c.l : 1 - c.l );
    double k = fmod(n + c.h / 30, 12);

    double b[] = {k - 3, 9 -k, 1};
    double d[] = {-1, m_min(b, 3)};
    return c.l - a * (d[0] > d[1] ? d[0] : d[1]);
}

struct colour get_rgb_from_hsl(struct colour c) {
    double r = magic_hsl_function(0, c);
    double g = magic_hsl_function(8, c);
    double b = magic_hsl_function(4, c);

    struct colour res;
    res.r = round(r * 255);
    res.g = round(g * 255);
    res.b = round(b * 255);
    return res;
}

struct colour get_rgb(struct colour c) {
    if (c.sp & CS_RGB){
        return c;
    }
    if (c.sp & CS_HSL) {
        return get_rgb_from_hsl(c);
    }
    if (c.sp & CS_HSV) {
        return get_rgb_from_hsv(c);
    }
    return c;
}

int compare_perceived_lum(const void* c1, const void* c2) {
    struct colour rgb_c1 = *(struct colour*)c1;
    struct colour rgb_c2 = *(struct colour*)c2;

    double lum1 = 0.299*rgb_c1.r + 0.587*rgb_c1.g + 0.114*rgb_c1.b;
    double lum2 = 0.299*rgb_c2.r + 0.587*rgb_c2.g + 0.114*rgb_c2.b;
    return lum1 - lum2;
}

struct colour *get_adjacent(struct colour base, int deg, int num) {
    struct colour col = get_hsl(base);
    num += 1;

    struct colour* colours = alloca(sizeof(struct colour) * num); 

    for (int i = 0; i < num; i++) {
        int m = (i % 2 == 0) ? -i : i;

        colours[i] = col;
        colours[i].h += m * deg;
        colours[i].h += fmod(colours[i].h, 360);
    }

    struct colour *ret_colours = calloc(num, sizeof(struct colour) * num);

    for (int i = 0; i < num; i++) {
        ret_colours[i] = get_rgb(colours[i]);
    }

    // sort from dark to bright
    qsort(ret_colours, num, sizeof(struct colour), compare_perceived_lum);

    return ret_colours;
}

void print_colour(struct colour c) {
    char *colour = calloc(20, sizeof(char));
    sprintf(colour, "#%02x%02x%02x (%d)", c.r, c.g, c.b, c.a);
    
    printf("\x1b[38;2;%d;%d;%dm%s\x1b[0m\n", c.r, c.g, c.b, colour);
    free(colour);
}

void test(int seed) {
    struct colour c = get_random_color();
    //print_colour(c);
    struct colour hsl = get_hsl(c);
    struct colour hsv = get_hsv(c);
    /*printf("RGB: %d %d %d\n",c.r,c.g,c.b);*/
    /*printf("HSL: %f %f %f\n",hsl.h, hsl.s, hsl.l);*/
    /*printf("HSV: %f %f %f\n",hsv.h, hsv.s, hsv.v);*/

    struct colour *adj = get_adjacent(c, 5, 4);
    for (int i = 0; i < 5; i++) {
        print_colour(adj[i]);
    }

    free(adj);
    printf("\n");
}

void test_print_wheel() {
    struct colour c;
    c.s = 0.999;
    c.v = 0.99;
    c.l = 0.5;
    c.sp = CS_HSV;
    for(int i = 0; i < 360; i+=5) {
        c.h = (double)i;
        struct colour rgb = get_rgb(c);
        print_colour(rgb);

    }
}
