/* generated common header file - do not edit */
#ifndef COMMON_DATA_H_
#define COMMON_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "r_crypto_api.h"
#include "r_aes_api.h"
#include "r_rsa_api.h"
#include "r_ecc_api.h"
#include "r_hash_api.h"
#include "r_trng_api.h"
#include "r_sce.h"
#include "r_trng_api.h"
#include "r_elc.h"
#include "r_elc_api.h"
#include "r_ioport.h"
#include "r_ioport_api.h"
#include "r_fmi.h"
#include "r_fmi_api.h"
#include "r_cgc.h"
#include "r_cgc_api.h"
#ifdef __cplusplus
extern "C"
{
#endif
extern const crypto_instance_t g_sce;
#define R_SCE_SERVICES_AES_PLAIN_TEXT_128_ECB   (0)
#define R_SCE_SERVICES_AES_PLAIN_TEXT_128_CBC   (0)
#define R_SCE_SERVICES_AES_PLAIN_TEXT_128_CTR   (0)
#define R_SCE_SERVICES_AES_PLAIN_TEXT_128_GCM   (0)
#define R_SCE_SERVICES_AES_PLAIN_TEXT_128_XTS   (0)
#define R_SCE_SERVICES_AES_PLAIN_TEXT_192_ECB   (0)
#define R_SCE_SERVICES_AES_PLAIN_TEXT_192_CBC   (0)
#define R_SCE_SERVICES_AES_PLAIN_TEXT_192_CTR   (0)
#define R_SCE_SERVICES_AES_PLAIN_TEXT_192_GCM   (0)
#define R_SCE_SERVICES_AES_PLAIN_TEXT_256_ECB   (0)
#define R_SCE_SERVICES_AES_PLAIN_TEXT_256_CBC   (0)
#define R_SCE_SERVICES_AES_PLAIN_TEXT_256_CTR   (0)
#define R_SCE_SERVICES_AES_PLAIN_TEXT_256_GCM   (0)
#define R_SCE_SERVICES_AES_PLAIN_TEXT_256_XTS   (0)
#define R_SCE_SERVICES_AES_WRAPPED_128_ECB      (0)
#define R_SCE_SERVICES_AES_WRAPPED_128_CBC      (0)
#define R_SCE_SERVICES_AES_WRAPPED_128_CTR      (0)
#define R_SCE_SERVICES_AES_WRAPPED_128_GCM      (0)
#define R_SCE_SERVICES_AES_WRAPPED_128_XTS      (0)
#define R_SCE_SERVICES_AES_WRAPPED_192_ECB      (0)
#define R_SCE_SERVICES_AES_WRAPPED_192_CBC      (0)
#define R_SCE_SERVICES_AES_WRAPPED_192_CTR      (0)
#define R_SCE_SERVICES_AES_WRAPPED_192_GCM      (0)
#define R_SCE_SERVICES_AES_WRAPPED_256_ECB      (0)
#define R_SCE_SERVICES_AES_WRAPPED_256_CBC      (0)
#define R_SCE_SERVICES_AES_WRAPPED_256_CTR      (0)
#define R_SCE_SERVICES_AES_WRAPPED_256_GCM      (0)
#define R_SCE_SERVICES_AES_WRAPPED_256_XTS      (0)
#define R_SCE_SERVICES_RSA_PLAIN_TEXT_1024      (0)
#define R_SCE_SERVICES_RSA_PLAIN_TEXT_2048      (0)
#define R_SCE_SERVICES_RSA_WRAPPED_1024         (0)
#define R_SCE_SERVICES_RSA_WRAPPED_2048         (0)
#define R_SCE_SERVICES_ECC_PLAIN_TEXT_192       (0)
#define R_SCE_SERVICES_ECC_PLAIN_TEXT_256       (0)
#define R_SCE_SERVICES_ECC_WRAPPED_192          (0)
#define R_SCE_SERVICES_ECC_WRAPPED_256          (0)
#define R_SCE_SERVICES_HASH_SHA1                (0)
#define R_SCE_SERVICES_HASH_SHA256              ((0) || (0))
#define R_SCE_SERVICES_HASH_MD5                 (0)
#define R_SCE_SERVICES_TRNG                     (1)
extern const trng_instance_t g_sce_trng;
/** ELC Instance */
extern const elc_instance_t g_elc;
/** IOPORT Instance */
extern const ioport_instance_t g_ioport;
/** FMI on FMI Instance. */
extern const fmi_instance_t g_fmi;
/** CGC Instance */
extern const cgc_instance_t g_cgc;
void g_common_init(void);
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* COMMON_DATA_H_ */
