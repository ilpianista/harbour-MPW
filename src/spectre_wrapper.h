/*
 * spectre_wrapper.h
 *
 * A C++-safe facade over the spectre C library.
 * The spectre headers use C99/C11 constructs (spectre_enum, static array
 * parameters) that are not valid C++.  This header never includes any
 * spectre header; it only uses primitive types so it can be included
 * from any C++ translation unit without issues.
 *
 * The matching implementation (spectre_wrapper.c) is compiled as C and
 * is the only file that ever sees the real spectre headers.
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

/* -----------------------------------------------------------------------
 * SW_Identicon — C++-safe mirror of SpectreIdenticon.
 * The `color` field uses uint8_t instead of the SpectreIdenticonColor enum.
 * ----------------------------------------------------------------------- */
typedef struct
{
    const char *leftArm;
    const char *body;
    const char *rightArm;
    const char *accessory;
    uint8_t color; /* underlying value of SpectreIdenticonColor */
} SW_Identicon;

/* -----------------------------------------------------------------------
 * Algorithm version constants  (mirror SpectreAlgorithm enum values)
 * ----------------------------------------------------------------------- */
#define SW_ALGORITHM_V0 0u
#define SW_ALGORITHM_V1 1u
#define SW_ALGORITHM_V2 2u
#define SW_ALGORITHM_V3 3u
#define SW_ALGORITHM_CURRENT 3u

/* -----------------------------------------------------------------------
 * Key-purpose constants  (mirror SpectreKeyPurpose enum values)
 * ----------------------------------------------------------------------- */
#define SW_PURPOSE_AUTHENTICATION 0u
#define SW_PURPOSE_IDENTIFICATION 1u
#define SW_PURPOSE_RECOVERY 2u

/* -----------------------------------------------------------------------
 * Result-type constants  (mirror SpectreResultType computed enum values)
 *
 * Formula from spectre-types.h:
 *   SpectreResultClassTemplate = 1 << 4 = 16
 *   SpectreResultFeatureNone   = 0
 *   value = (index | SpectreResultClassTemplate | SpectreResultFeatureNone)
 * ----------------------------------------------------------------------- */
#define SW_RESULT_MAXIMUM 16u /* SpectreResultTemplateMaximum */
#define SW_RESULT_LONG 17u    /* SpectreResultTemplateLong    */
#define SW_RESULT_MEDIUM 18u  /* SpectreResultTemplateMedium  */
#define SW_RESULT_SHORT 19u   /* SpectreResultTemplateShort   */
#define SW_RESULT_BASIC 20u   /* SpectreResultTemplateBasic   */
#define SW_RESULT_PIN 21u     /* SpectreResultTemplatePIN     */
#define SW_RESULT_NAME 30u    /* SpectreResultTemplateName    */
#define SW_RESULT_PHRASE 31u  /* SpectreResultTemplatePhrase  */

/* Default site counter (SpectreCounterInitial = 1) */
#define SW_COUNTER_INITIAL 1u

/* -----------------------------------------------------------------------
 * Key buffer API
 *
 * Instead of exposing SpectreUserKey (which contains enum fields), callers
 * allocate an opaque byte buffer of size sw_user_key_size() and pass it to
 * sw_user_key_fill().  The buffer is then handed directly to sw_site_result().
 * It is safe to store this buffer in a QByteArray.
 * ----------------------------------------------------------------------- */

/** Returns sizeof(SpectreUserKey).  Allocate this many bytes for the key buffer. */
size_t sw_user_key_size(void);

/**
 * Derives the user key and writes the full SpectreUserKey struct into `out`.
 * `outSize` must be >= sw_user_key_size().
 * @return 1 on success, 0 on failure (bad credentials or allocation error).
 */
int sw_user_key_fill(const char *userName,
                     const char *userSecret,
                     unsigned int algorithm,
                     uint8_t *out,
                     size_t outSize);

/* -----------------------------------------------------------------------
 * Site-result generation
 * ----------------------------------------------------------------------- */

/**
 * Generate a password/result token.
 * `keyBuf` must point to a buffer previously filled by sw_user_key_fill().
 * @return An allocated C-string, or NULL on error. Caller must free() it.
 */
const char *sw_site_result(const uint8_t *keyBuf,
                           const char *siteName,
                           uint32_t resultType,
                           const char *resultParam,
                           uint32_t keyCounter,
                           uint8_t keyPurpose,
                           const char *keyContext);

/* -----------------------------------------------------------------------
 * Identicon helpers
 * ----------------------------------------------------------------------- */

/** Generate an identicon for the given credentials. */
SW_Identicon sw_identicon(const char *userName, const char *userSecret);

/**
 * Encode an identicon to a display string.
 * @return An allocated C-string, or NULL if the identicon is unset.
 */
const char *sw_identicon_encode(SW_Identicon identicon);

#ifdef __cplusplus
}
#endif
