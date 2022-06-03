#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//#include "bn.h"
enum bn_codes {
    BN_OK, BN_NULL_OBJECT, BN_NO_MEMORY, BN_DIVIDE_BY_ZERO
};

const size_t BASE = 1ull << (sizeof(size_t) / 2 * 8);

struct bn_s {
    size_t n;
    size_t *a;
    short sign;
};

static int bn_is_OK(bn const *t) {
    if (t == NULL || t->a == NULL)
        return BN_NULL_OBJECT;
    return BN_OK;
}

static void print(const bn *t) {
    if (t->sign < 0)
        printf("-");
    for (size_t i=0; i<(t->n); i++)
        printf("%zu ", t->a[i]);
    printf("\n");
}

bn *bn_new() {
    bn *num = (bn *)malloc(sizeof (bn));
    if (num == NULL) return NULL;
    num->a = (size_t *)malloc(sizeof(size_t));
    if (num->a == NULL)  {
        free(num);
        return NULL;
    }
    num->a[0] = 0;
    num->n = 1;
    num->sign = 1;
    return num;
}

bn *bn_init(bn const *orig) {
    if (bn_is_OK(orig)) return NULL;
    bn *copy = bn_new();
    if (copy == NULL) return NULL;
    copy->n = orig->n;
    copy->sign = orig->sign;
    copy->a = (size_t *)realloc(copy->a, sizeof(size_t) * copy->n);
    if (bn_is_OK(copy)) {
        bn_delete(copy);
        return NULL;
    }
    for (size_t i=0; i<copy->n; i++)
        copy->a[i] = orig->a[i];
    return copy;
}

static int bn_expand(bn *t, size_t new_n) {
    if (new_n < t->n) return BN_OK;
    t->a = (size_t *)realloc(t->a, sizeof(size_t) * new_n);
    if (bn_is_OK(t)) return BN_NO_MEMORY;
    for (size_t i=t->n; i<new_n; i++)
        t->a[i] = 0;
    t->n = new_n;
    return BN_OK;
}

int bn_delete(bn *t) {
    if (t != NULL)
        free(t->a);
    free(t);
    return BN_OK;
}

static int bn_update(bn *t, const bn *new_value) {
    t->n = new_value->n;
    t->sign = new_value->sign;
    t->a = (size_t *)realloc(t->a, sizeof(size_t) * t->n);
    if (bn_is_OK(t)) return BN_NO_MEMORY;
    for (size_t i=0; i<t->n; i++)
        t->a[i] = new_value->a[i];
    return BN_OK;
}

static int bn_clear_begin(bn *t) {
    for (; t->n > 1 && !(t->a[t->n-1] || t->a[t->n-2]); t->n--);
    t->a = (size_t *)realloc(t->a, sizeof(size_t) * t->n);
    if (bn_is_OK(t)) return BN_NO_MEMORY;
    if (t->n == 1 && t->a[0] == 0) t->sign = 1;
    if (t->a[t->n-1] != 0) {
        if (bn_expand(t, t->n + 1)) return BN_NO_MEMORY;
        t->a[t->n-1] = 0;
    }
    return 0;
}

int bn_init_int(bn *t, int init_int) {
    if (!init_int) return BN_OK;
    t->sign = (init_int >= 0) ? 1 : -1;
    size_t n = 2;
    if (bn_expand(t, n)) return BN_NO_MEMORY;
    t->a[0] = (size_t)abs(init_int);
    return BN_OK;
}

static int bn_int_mul_to(bn *t, size_t x) {
    if (bn_expand(t, t->n+1)) return BN_NO_MEMORY;
    size_t hash = 0;
    for (size_t i = 0; i < t->n; i++) {
        t->a[i] = t->a[i] * x + hash;
        if (t->a[i] >= BASE) {
            hash = t->a[i] / BASE;
            t->a[i] %= BASE;
        }
        else
            hash = 0;
    }
    return bn_clear_begin(t);
}

static size_t bn_int_div_to_and_get_mod(bn *t, size_t x) {
    size_t mod = 0, old_mod = 0;
    for (size_t i = t->n - 1; i + 1 >= 1; i--) {
        mod = (t->a[i] + old_mod * BASE) % x;
        t->a[i] = (t->a[i] + old_mod * BASE) / x;
        old_mod = mod;
    }
    bn_clear_begin(t);
    return mod;
}

int bn_init_string_radix(bn *t, char const *init_string, int radix) {
    if (bn_is_OK(t) || radix < 2 || init_string == NULL) return BN_NULL_OBJECT;
    size_t str_len = 0;
    for (; init_string[str_len] != '\0' && init_string[str_len] != '\n'; str_len++);
    if (str_len == 0) return BN_NULL_OBJECT;
    size_t i = 0;
    if (init_string[0] == '-') {
        t->sign = -1;
        i++;
    }
    if (i == str_len || init_string[i] < '0' || init_string[i] > '9') return BN_NULL_OBJECT;
    t->a[0] += (size_t)(init_string[i] - '0');
    i++;
    for (; i<str_len; i++) {
        if (init_string[i] < '0' || init_string[i] > '9') {
            free(t->a);
            return BN_NULL_OBJECT;
        }
        if (bn_int_mul_to(t, radix)) {
            free(t->a);
            return BN_NO_MEMORY;
        }
        t->a[0] += (size_t)(init_string[i] - '0');
    }
    if (bn_int_mul_to(t, 1)) {
        free(t->a);
        return BN_NO_MEMORY;
    }
    if (bn_clear_begin(t)) {
        free(t->a);
        return BN_NO_MEMORY;
    }
    return BN_OK;
}

int bn_init_string(bn *t, char const *init_string) {
    return bn_init_string_radix(t, init_string, 10);
}

const char *bn_to_string(bn const *t, int radix) {
    if (bn_is_OK(t) || radix < 2) return NULL;
    bn *bn_for_div = bn_init(t);
    if (bn_is_OK(bn_for_div)) {
        bn_delete(bn_for_div);
        return NULL;
    }
    bn_for_div->sign = 1;
    bn *zero = bn_new();
    if (bn_is_OK(zero)) {
        bn_delete(zero);
        return NULL;
    }
    int minus = (t->sign < 0);
    size_t len;
    char *str;
    if (t->n == 1 && t->a[0] == 0) {
        len = 1;
        str = (char *)malloc(sizeof(char) * len);
        str[0] = '0';
    }
    else {
        len = (t->n - 1) * (size_t)(floor(log2((double) BASE) / log2((double) radix)) + 1);
        str = (char *) malloc(sizeof(char) * len);
        if (str == NULL) return NULL;
        size_t i = 0;
        for (; bn_cmp(bn_for_div, zero) != 0; i++) {
            size_t digit = bn_int_div_to_and_get_mod(bn_for_div, radix);
            if (digit < 10)
                str[i] = '0' + (char)digit;
            else
                str[i] = 'A' + (char)(digit - 10);
        }
        len = i + minus;
    }
    char *res = (char *)malloc(sizeof(char) * (len + 1));
    if (res == NULL) return NULL;
    for (size_t j = 0; j < len - minus; j++)
        res[len-j-1] = str[j];
    if (minus) res[0] = '-';
    res[len] = '\0';
    free(str);
    bn_delete(bn_for_div);
    bn_delete(zero);
    const char *final_res = res;
    return final_res;
}

static void pretty_print(const bn *t) {
    const char *s = bn_to_string(t, 10);
    printf("%s\n", s);
//    free(s);
}

int bn_neg(bn *t) {
    if (bn_is_OK(t)) return BN_NULL_OBJECT;
    t->sign *= -1;
    return BN_OK;
}
int bn_abs(bn *t) {
    if (bn_is_OK(t)) return BN_NULL_OBJECT;
    t->sign = 1;
    return BN_OK;
}
int bn_sign(bn const *t) {
    if (t->n == 1 && t->a[0] == 0)
        return 0;
    else return t->sign;
}

static int bn_cmp_abs(bn const *left, bn const *right) {
    if (left->n < right->n)
        return -1;
    else if (left->n > right->n)
        return 1;
    for (size_t i = left->n - 1; i + 1 >= 1; i--) {
        if (left->a[i] < right->a[i]) return -1;
        else if (left->a[i] > right->a[i]) return 1;
    }
    return 0;
}

int bn_cmp(bn const *left, bn const *right) {
    if (left->sign < 0 && right->sign > 0)
        return -1;
    if (left->sign > 0 && right->sign < 0)
        return 1;
    return bn_cmp_abs(left, right) * left->sign;
}

int bn_add_to(bn *t, bn const *right) {
    if (bn_is_OK(t) || bn_is_OK(right)) return BN_NULL_OBJECT;
    if (t->sign < 0 || right->sign < 0) {
        bn *copy_right = bn_init(right);
        if (copy_right == NULL) return BN_NO_MEMORY;
        copy_right->sign *= -1;
        int code = bn_sub_to(t, copy_right);
        bn_delete(copy_right);
        return code;
    }
    if (bn_expand(t, (right->n > t->n) ? right->n+1 : t->n+1)) return BN_NO_MEMORY;
    size_t hash = 0;
    for (size_t i = 0; i < right->n; i++) {
        t->a[i] += right->a[i] + hash;
        hash = t->a[i] / BASE;
        t->a[i] %= BASE;
    }
    for (size_t i = right->n; hash && i < t->n; i++) {
        t->a[i] += hash;
        hash = t->a[i] / BASE;
        t->a[i] %= BASE;
    }
    if (bn_clear_begin(t)) return BN_NO_MEMORY;
    return BN_OK;
}

bn* bn_add(bn const *left, bn const *right) {
    bn *result = bn_init(left);
    int code = bn_add_to(result, right);
    if (code)
        return NULL;
    return result;
}

int bn_sub_to(bn *t, bn const *right) {
    if (bn_is_OK(t) || bn_is_OK(right)) return BN_NULL_OBJECT;
    bn *maxpart, *minpart;
    bn const *maxpart_parent;
    if (bn_cmp_abs(t, right) < 0) {
        maxpart = bn_init(right);
        maxpart_parent = right;
        minpart = bn_init(t);
    } else {
        maxpart = bn_init(t);
        maxpart_parent = t;
        minpart = bn_init(right);
    }
    if (bn_is_OK(maxpart)) return BN_NO_MEMORY;
    if (bn_is_OK(minpart)) {
        bn_delete(maxpart);
        return BN_NO_MEMORY;
    }
    bn_abs(maxpart);
    bn_abs(minpart);
    if (t->sign != right->sign) {
        int code = bn_add_to(maxpart, minpart);
        if (code) {
            bn_delete(maxpart);
            bn_delete(minpart);
            return code;
        }
        short future_sign = t->sign;
        code = bn_update(t, maxpart);
        t->sign = future_sign;
        bn_delete(maxpart);
        bn_delete(minpart);
        return code;
    }
    short future_sign;
    if (t->sign == 1 && right->sign == 1)
        future_sign = (maxpart_parent == t) ? 1 : -1;
    else if (maxpart_parent == t)
        future_sign = -1;
    else
        future_sign = 1;
    int hash = 0;
    if (bn_expand(maxpart, maxpart->n+1)) {
        bn_delete(maxpart);
        bn_delete(minpart);
        return BN_NO_MEMORY;
    }
    for (size_t i = 0; i < minpart->n; i++) {
        if (maxpart->a[i] >= minpart->a[i] + hash) {
            maxpart->a[i] -= minpart->a[i] + hash;
            hash = 0;
        }
        else {
            maxpart->a[i] += BASE - minpart->a[i] - hash;
            hash = 1;
        }
    }
    for (size_t i = minpart->n; i < maxpart->n; i++) {
        if (maxpart->a[i] >= hash) {
            maxpart->a[i] -= hash;
            hash = 0;
        }
        else {
            maxpart->a[i] += BASE - hash;
            hash = 1;
        }
    }
    int code = bn_update(t, maxpart);
    bn_delete(maxpart);
    bn_delete(minpart);
    t->sign = future_sign;
    if (bn_clear_begin(t)) return BN_NO_MEMORY;
    return code;
}

bn* bn_sub(bn const *left, bn const *right) {
    bn *result = bn_init(left);
    if (bn_is_OK(result)) return NULL;
    if (bn_sub_to(result, right)) return NULL;
    return result;
}

static int bn_mul_to_main(bn *t, bn const *right) {
    bn *old_t = bn_init(t);
    if (bn_is_OK(old_t)) {
        bn_delete(old_t);
        return BN_NO_MEMORY;
    }
    t->sign *= right->sign;
    if (bn_expand(t, t->n + right->n)) {
        bn_delete(old_t);
        return BN_NO_MEMORY;
    }
    for (size_t i = 0; i < t->n; i++)
        t->a[i] = 0;
    size_t dig, hash, final_hash, old_final_hash;
    for (size_t j = 0; j < right->n; j++) {
        hash = final_hash = old_final_hash = 0;
        for (size_t i = 0; i < old_t->n; i++) {
            dig = old_t->a[i] * right->a[j] + hash;
            hash = dig / BASE;
            final_hash = (t->a[i + j] + dig % BASE + final_hash) / BASE;
            t->a[i + j] = (t->a[i + j] + dig % BASE + old_final_hash) % BASE;
            old_final_hash = final_hash;
        }
    }
    bn_delete(old_t);
    if (bn_clear_begin(t)) return BN_NO_MEMORY;
    return BN_OK;
}

int bn_mul_to(bn *t, bn const *right) {
    if (bn_is_OK(t) || bn_is_OK(right)) return BN_NULL_OBJECT;
    if (t != right) {
        return bn_mul_to_main(t, right);
    }
    bn *copy_t = bn_init(t);
    if (bn_is_OK(copy_t)) {
        bn_delete(copy_t);
        return BN_NO_MEMORY;
    }
    int code = bn_mul_to_main(t, copy_t);
    bn_delete(copy_t);
    return code;
}

bn* bn_mul(bn const *left, bn const *right) {
    bn *result = bn_init(left);
    if (bn_is_OK(result)) {
        bn_delete(result);
        return NULL;
    }
    if (bn_mul_to(result, right)) {
        bn_delete(result);
        return NULL;
    }
    return result;
}

int bn_pow_to(bn *t, int n) {
    if (bn_is_OK(t) || n < 0) return BN_NULL_OBJECT;
    if (n == 0) {
        bn *one = bn_new();
        if (bn_is_OK(one)) {
            bn_delete(one);
            return BN_NO_MEMORY;
        }
        if (bn_init_int(one, 1)) {
            bn_delete(one);
            return BN_NO_MEMORY;
        }
        int code = bn_update(t, one);
        bn_delete(one);
        return code;
    }
    if (n == 1)
        return BN_OK;
    if (n % 2 == 0) {
        int code = bn_pow_to(t, n / 2);
        if (code) return code;
        return bn_mul_to(t, t);
    }
    int good_n = 1u << (int)log2(n);
    bn *old_t = bn_init(t);
    if (bn_is_OK(old_t)) {
        bn_delete(old_t);
        return BN_NO_MEMORY;
    }
    int code = bn_pow_to(t, good_n);
    if (code) {
        bn_delete(old_t);
        return code;
    }
    for (size_t i = 0; i < n - good_n; i++) {
        code = bn_mul_to(t, old_t);
        if (code) {
            bn_delete(old_t);
            return code;
        }
    }
    bn_delete(old_t);
    return BN_OK;
}

static size_t bn_easy_div(bn *t, bn const *right) {
    if (bn_cmp_abs(t, right) < 0)
        return 0;
    size_t l = 1, r = BASE - 1, m;
    bn *sum_for_check = bn_init(right);
    bn_abs(sum_for_check);
    while(r - l > 1) {
        m = (l + r) / 2;
        bn_int_mul_to(sum_for_check, m);
        if (bn_cmp_abs(sum_for_check, t) < 0)
            l = m;
        else if (bn_cmp_abs(t, sum_for_check) < 0)
            r = m;
        else
            l = r = m;
        bn_update(sum_for_check, right);
        bn_abs(sum_for_check);
    }
    size_t res;
    bn_int_mul_to(sum_for_check, r);
    if (bn_cmp_abs(sum_for_check, t) <= 0) {
        res = r;
    }
    else {
        bn_update(sum_for_check, right);
        bn_abs(sum_for_check);
        bn_int_mul_to(sum_for_check, l);
        res = l;
    }
    bn_sub_to(t, sum_for_check);
    bn_delete(sum_for_check);
    return res;
}

static int wrapped_bn_div_and_mod(bn *t, bn const *right, int for_mod, bn *mod) {
    int future_sign = t->sign;
    bn_abs(t);
    bn *result = bn_new();
    if (bn_is_OK(result)) {
        bn_delete(result);
        return BN_NO_MEMORY;
    }
    if (bn_cmp_abs(result, right) == 0) {
        bn_delete(result);
        return BN_DIVIDE_BY_ZERO;
    }
    if (bn_cmp_abs(t, right) < 0) {
        if (!for_mod)
            bn_update(t, result);
        bn_delete(result);
        return (bn_is_OK(t) ? BN_NO_MEMORY : BN_OK);
    }
    bn_expand(result, t->n);
    if (bn_is_OK(result)) {
        bn_delete(result);
        return BN_NO_MEMORY;
    }
    bn *cur_sum = bn_new();
    if (bn_is_OK(cur_sum)) {
        bn_delete(result);
        bn_delete(cur_sum);
        return BN_NO_MEMORY;
    }
    int code;
    for (size_t i = t->n - 1; i + 1 >= 1; i--) {
        code = bn_int_mul_to(cur_sum, BASE);
        if (code) {
            bn_delete(result);
            bn_delete(cur_sum);
            return code;
        }
        cur_sum->a[0] += t->a[i];
        bn_clear_begin(cur_sum);
        result->a[i] = bn_easy_div(cur_sum, right);
        if (bn_is_OK(cur_sum)) {
            bn_delete(result);
            bn_delete(cur_sum);
            return BN_NO_MEMORY;
        }
    }
    bn_clear_begin(result);
    code = bn_update(mod, cur_sum);
    if (code) {
        bn_delete(result);
        bn_delete(cur_sum);
        return code;
    }
    if (for_mod) {
        code = bn_update(t, cur_sum);
    } else {
        code = bn_update(t, result);
    }
    if (t->n == 1 && t->a[0] == 0)
        bn_abs(t);
    else
        t->sign = future_sign;
    bn_delete(result);
    bn_delete(cur_sum);
    return code;
}

int bn_div_to(bn *t, bn const *right) {
    if (bn_is_OK(t) || bn_is_OK(right)) return BN_NULL_OBJECT;
    bn *mod = bn_new();
    if (bn_is_OK(mod)) {
        bn_delete(mod);
        return BN_NO_MEMORY;
    }
    int code = wrapped_bn_div_and_mod(t, right, 0, mod);
    if (code) return code;
    t->sign *= right->sign;
    if (t->sign < 0 && !(mod->n == 1 || mod->a[0] == 0)) {
        t->a[0] += 1;
        code = bn_int_mul_to(t, 1);
    }
    bn_delete(mod);
    return code;
}

bn* bn_div(bn const *left, bn const *right) {
    bn *result = bn_init(left);
    if (bn_is_OK(result)) {
        bn_delete(result);
        return NULL;
    }
    if (bn_div_to(result, right)) {
        bn_delete(result);
        return NULL;
    }
    return result;
}

int bn_mod_to(bn *t, bn const *right) {
    if (bn_is_OK(t) || bn_is_OK(right)) return BN_NULL_OBJECT;
    bn *mod = bn_new();
    if (bn_is_OK(mod)) {
        bn_delete(mod);
        return BN_NO_MEMORY;
    }
    int code = wrapped_bn_div_and_mod(t, right, 1, mod);
    if (code) return code;
    if (t->sign * right->sign < 0 && !(mod->n == 1 && mod->a[0] == 0)) {
        t->sign = -1 * right->sign;
        code = bn_add_to(t, right);
    }
    bn_delete(mod);
    return code;
}

bn* bn_mod(bn const *left, bn const *right) {
    bn *result = bn_init(left);
    if (bn_is_OK(result)) {
        bn_delete(result);
        return NULL;
    }
    if (bn_mod_to(result, right)) {
        bn_delete(result);
        return NULL;
    }
    return result;
}

int bn_root_to(bn *t, int n) {
    //Without Exception Handling
    bn *part1 = bn_new();
    bn_init_int(part1, 1);
    bn_int_mul_to(part1, BASE);
    bn_pow_to(part1, ceil(((double)t->n) / ((double)n)));
    bn *x = bn_new(), *part2 = bn_new(), *denum = bn_new();
    bn_init_int(x, -1);
    while (bn_cmp_abs(part1, x)) {
        bn_update(x, part1);
        bn_int_mul_to(part1, n - 1);
        bn_update(part2, t);
        bn_update(denum, x);
        bn_pow_to(denum, n - 1);
        bn_div_to(part2, denum);
        bn_add_to(part1, part2);
        bn_int_div_to_and_get_mod(part1, n);
    }
    bn_update(t, x);
    bn_delete(x);
    bn_delete(part1);
    bn_delete(part2);
    bn_delete(denum);
    return BN_OK;
}
