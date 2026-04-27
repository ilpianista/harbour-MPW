/*
 * spectre_wrapper.c
 *
 * Compiled as C (not C++).  This is the only translation unit that
 * includes the spectre C headers; all C++ code uses spectre_wrapper.h
 * instead.
 */

#include "spectre_wrapper.h"

/* Pull in the real spectre API — valid only in C compilation */
#include <spectre-algorithm.h>
#include <spectre-util.h>

#include <stdlib.h>
#include <string.h>

/* -----------------------------------------------------------------------
 * Key buffer
 * ----------------------------------------------------------------------- */

size_t sw_user_key_size(void)
{
    return sizeof(SpectreUserKey);
}

int sw_user_key_fill(const char *userName,
                     const char *userSecret,
                     unsigned int algorithm,
                     uint8_t *out,
                     size_t outSize)
{
    if (!out || outSize < sizeof(SpectreUserKey))
        return 0;

    const SpectreUserKey *k = spectre_user_key(userName, userSecret, (SpectreAlgorithm) algorithm);
    if (!k)
        return 0;

    memcpy(out, k, sizeof(SpectreUserKey));
    free((void *) k);
    return 1;
}

/* -----------------------------------------------------------------------
 * Site-result generation
 * ----------------------------------------------------------------------- */

const char *sw_site_result(const uint8_t *keyBuf,
                           const char *siteName,
                           uint32_t resultType,
                           const char *resultParam,
                           uint32_t keyCounter,
                           uint8_t keyPurpose,
                           const char *keyContext)
{
    /*
     * The buffer was filled with a verbatim copy of SpectreUserKey, so it
     * is safe to cast it back to SpectreUserKey* within this C translation
     * unit where the struct layout is fully known.
     */
    const SpectreUserKey *k = (const SpectreUserKey *) keyBuf;

    return spectre_site_result(k,
                               siteName,
                               (SpectreResultType) resultType,
                               resultParam,
                               (SpectreCounter) keyCounter,
                               (SpectreKeyPurpose) keyPurpose,
                               keyContext);
}

/* -----------------------------------------------------------------------
 * Identicon helpers
 * ----------------------------------------------------------------------- */

SW_Identicon sw_identicon(const char *userName, const char *userSecret)
{
    SpectreIdenticon id = spectre_identicon(userName, userSecret);
    SW_Identicon out;
    out.leftArm = id.leftArm;
    out.body = id.body;
    out.rightArm = id.rightArm;
    out.accessory = id.accessory;
    out.color = (uint8_t) id.color;
    return out;
}

const char *sw_identicon_encode(SW_Identicon identicon)
{
    SpectreIdenticon id;
    id.leftArm = identicon.leftArm;
    id.body = identicon.body;
    id.rightArm = identicon.rightArm;
    id.accessory = identicon.accessory;
    id.color = (SpectreIdenticonColor) identicon.color;
    return spectre_identicon_encode(id);
}
