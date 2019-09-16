/* generated common source file - do not edit */
#include "common_data.h"
#if defined(BSP_FEATURE_HAS_SCE_ON_S1)  /* Crypto on S1 */
sce_interface_get_api_interfaces_t g_sce_selected_api_interfaces =
{
#if R_SCE_SERVICES_AES_PLAIN_TEXT_128_ECB
    .aes128ecb_on_sce = (uint32_t)&g_aes128ecb_on_sce,
#else
    .aes128ecb_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_128_CBC
    .aes128cbc_on_sce = (uint32_t)&g_aes128cbc_on_sce,
#else
    .aes128cbc_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_128_CTR
    .aes128ctr_on_sce = (uint32_t)&g_aes128ctr_on_sce,
#else
    .aes128ctr_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_256_ECB
    .aes256ecb_on_sce = (uint32_t)&g_aes256ecb_on_sce,
#else
    .aes256ecb_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_256_CBC
    .aes256cbc_on_sce = (uint32_t)&g_aes256cbc_on_sce,
#else
    .aes256cbc_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_256_CTR
    .aes256ctr_on_sce = (uint32_t)&g_aes256ctr_on_sce,
#else
    .aes256ctr_on_sce = 0,
#endif
#if R_SCE_SERVICES_TRNG
    .trng_on_sce = (uint32_t)&g_trng_on_sce
#else
    .trng_on_sce = 0
#endif
};

#elif defined(BSP_FEATURE_HAS_SCE5) /* SCE5 */

sce_interface_get_api_interfaces_t g_sce_selected_api_interfaces =
{
#if R_SCE_SERVICES_AES_PLAIN_TEXT_128_ECB
    .aes128ecb_on_sce = (uint32_t)&g_aes128ecb_on_sce,
#else
    .aes128ecb_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_128_CBC
    .aes128cbc_on_sce = (uint32_t)&g_aes128cbc_on_sce,
#else
    .aes128cbc_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_128_CTR
    .aes128ctr_on_sce = (uint32_t)&g_aes128ctr_on_sce,
#else
    .aes128ctr_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_128_GCM
    .aes128gcm_on_sce = (uint32_t)&g_aes128gcm_on_sce,
#else
    .aes128gcm_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_128_XTS
    .aes128xts_on_sce = (uint32_t)&g_aes128xts_on_sce,
#else
    .aes128xts_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_256_ECB
    .aes256ecb_on_sce = (uint32_t)&g_aes256ecb_on_sce,
#else
    .aes256ecb_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_256_CBC
    .aes256cbc_on_sce = (uint32_t)&g_aes256cbc_on_sce,
#else
    .aes256cbc_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_256_CTR
    .aes256ctr_on_sce = (uint32_t)&g_aes256ctr_on_sce,
#else
    .aes256ctr_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_256_GCM
    .aes256gcm_on_sce = (uint32_t)&g_aes256gcm_on_sce,
#else
    .aes256gcm_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_256_XTS
    .aes256xts_on_sce = (uint32_t)&g_aes256xts_on_sce,
#else
    .aes256xts_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_128_ECB
    .aes128ecb_on_sceHrk = (uint32_t)&g_aes128ecb_on_sceHrk,
#else
    .aes128ecb_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_128_CBC
    .aes128cbc_on_sceHrk = (uint32_t)&g_aes128cbc_on_sceHrk,
#else
    .aes128cbc_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_128_CTR
    .aes128ctr_on_sceHrk = (uint32_t)&g_aes128ctr_on_sceHrk,
#else
    .aes128ctr_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_128_GCM
    .aes128gcm_on_sceHrk = (uint32_t)&g_aes128gcm_on_sceHrk,
#else
    .aes128gcm_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_128_XTS
    .aes128xts_on_sceHrk = (uint32_t)&g_aes128xts_on_sceHrk,
#else
    .aes128xts_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_256_ECB
    .aes256ecb_on_sceHrk = (uint32_t)&g_aes256ecb_on_sceHrk,
#else
    .aes256ecb_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_256_CBC
    .aes256cbc_on_sceHrk = (uint32_t)&g_aes256cbc_on_sceHrk,
#else
    .aes256cbc_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_256_CTR
    .aes256ctr_on_sceHrk = (uint32_t)&g_aes256ctr_on_sceHrk,
#else
    .aes256ctr_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_256_GCM
    .aes256gcm_on_sceHrk = (uint32_t)&g_aes256gcm_on_sceHrk,
#else
    .aes256gcm_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_256_XTS
    .aes256xts_on_sceHrk = (uint32_t)&g_aes256xts_on_sceHrk,
#else
    .aes256xts_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_TRNG
    .trng_on_sce = (uint32_t)&g_trng_on_sce
#else
    .trng_on_sce = 0
#endif
};

#else /* SCE7 */

sce_interface_get_api_interfaces_t g_sce_selected_api_interfaces =
{
#if R_SCE_SERVICES_AES_PLAIN_TEXT_128_ECB
  .aes128ecb_on_sce = (uint32_t)&g_aes128ecb_on_sce,
#else
  .aes128ecb_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_128_CBC
  .aes128cbc_on_sce = (uint32_t)&g_aes128cbc_on_sce,
#else
  .aes128cbc_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_128_CTR
  .aes128ctr_on_sce = (uint32_t)&g_aes128ctr_on_sce,
#else
  .aes128ctr_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_128_GCM
  .aes128gcm_on_sce = (uint32_t)&g_aes128gcm_on_sce,
#else
  .aes128gcm_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_128_XTS
  .aes128xts_on_sce = (uint32_t)&g_aes128xts_on_sce,
#else
  .aes128xts_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_192_ECB
  .aes192ecb_on_sce = (uint32_t)&g_aes192ecb_on_sce,
#else
  .aes192ecb_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_192_CBC
  .aes192cbc_on_sce = (uint32_t)&g_aes192cbc_on_sce,
#else
  .aes192cbc_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_192_CTR
  .aes192ctr_on_sce = (uint32_t)&g_aes192ctr_on_sce,
#else
  .aes192ctr_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_192_GCM
  .aes192gcm_on_sce = (uint32_t)&g_aes192gcm_on_sce,
#else
  .aes192gcm_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_256_ECB
  .aes256ecb_on_sce = (uint32_t)&g_aes256ecb_on_sce,
#else
  .aes256ecb_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_256_CBC
  .aes256cbc_on_sce = (uint32_t)&g_aes256cbc_on_sce,
#else
  .aes256cbc_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_256_CTR
  .aes256ctr_on_sce = (uint32_t)&g_aes256ctr_on_sce,
#else
  .aes256ctr_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_256_GCM
  .aes256gcm_on_sce = (uint32_t)&g_aes256gcm_on_sce,
#else
  .aes256gcm_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_PLAIN_TEXT_256_XTS
  .aes256xts_on_sce = (uint32_t)&g_aes256xts_on_sce,
#else
  .aes256xts_on_sce = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_128_ECB
  .aes128ecb_on_sceHrk = (uint32_t)&g_aes128ecb_on_sceHrk,
#else
  .aes128ecb_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_128_CBC
  .aes128cbc_on_sceHrk = (uint32_t)&g_aes128cbc_on_sceHrk,
#else
  .aes128cbc_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_128_CTR
  .aes128ctr_on_sceHrk = (uint32_t)&g_aes128ctr_on_sceHrk,
#else
  .aes128ctr_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_128_GCM
  .aes128gcm_on_sceHrk = (uint32_t)&g_aes128gcm_on_sceHrk,
#else
  .aes128gcm_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_128_XTS
  .aes128xts_on_sceHrk = (uint32_t)&g_aes128xts_on_sceHrk,
#else
  .aes128xts_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_192_ECB
  .aes192ecb_on_sceHrk = (uint32_t)&g_aes192ecb_on_sceHrk,
#else
  .aes192ecb_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_192_CBC
  .aes192cbc_on_sceHrk = (uint32_t)&g_aes192cbc_on_sceHrk,
#else
  .aes192cbc_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_192_CTR
  .aes192ctr_on_sceHrk = (uint32_t)&g_aes192ctr_on_sceHrk,
#else
  .aes192ctr_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_192_GCM
  .aes192gcm_on_sceHrk = (uint32_t)&g_aes192gcm_on_sceHrk,
#else
  .aes192gcm_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_256_ECB
  .aes256ecb_on_sceHrk = (uint32_t)&g_aes256ecb_on_sceHrk,
#else
  .aes256ecb_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_256_CBC
  .aes256cbc_on_sceHrk = (uint32_t)&g_aes256cbc_on_sceHrk,
#else
  .aes256cbc_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_256_CTR
  .aes256ctr_on_sceHrk = (uint32_t)&g_aes256ctr_on_sceHrk,
#else
  .aes256ctr_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_256_GCM
  .aes256gcm_on_sceHrk = (uint32_t)&g_aes256gcm_on_sceHrk,
#else
  .aes256gcm_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_AES_WRAPPED_256_XTS
  .aes256xts_on_sceHrk = (uint32_t)&g_aes256xts_on_sceHrk,
#else
  .aes256xts_on_sceHrk = 0,
#endif
#if R_SCE_SERVICES_RSA_PLAIN_TEXT_1024
  .rsa1024_on_sce = (uint32_t)&g_rsa1024_on_sce,
#else
  .rsa1024_on_sce = 0,
#endif
#if R_SCE_SERVICES_RSA_PLAIN_TEXT_2048
  .rsa2048_on_sce = (uint32_t)&g_rsa2048_on_sce,
#else
  .rsa2048_on_sce = 0,
#endif
#if R_SCE_SERVICES_RSA_WRAPPED_1024
  .rsa1024_on_sce_hrk = (uint32_t)&g_rsa1024_on_sce_hrk,
#else
  .rsa1024_on_sce_hrk = 0,
#endif
#if R_SCE_SERVICES_RSA_WRAPPED_2048
  .rsa2048_on_sce_hrk = (uint32_t)&g_rsa2048_on_sce_hrk,
#else
  .rsa2048_on_sce_hrk = 0,
#endif
#if R_SCE_SERVICES_HASH_MD5
  .md5_hash_on_sce = (uint32_t)&g_md5_hash_on_sce,
#else
  .md5_hash_on_sce = 0,
#endif
#if R_SCE_SERVICES_HASH_SHA1
  .sha1_hash_on_sce = (uint32_t)&g_sha1_hash_on_sce,
#else
  .sha1_hash_on_sce = 0,
#endif
#if R_SCE_SERVICES_HASH_SHA256
  .sha256_hash_on_sce = (uint32_t)&g_sha256_hash_on_sce,
#else
  .sha256_hash_on_sce = 0,
#endif
#if R_SCE_SERVICES_ECC_PLAIN_TEXT_192
  .ecc192_on_sce = (uint32_t)&g_ecc192_on_sce,
#else
  .ecc192_on_sce = 0,
#endif
#if R_SCE_SERVICES_ECC_PLAIN_TEXT_256
  .ecc256_on_sce = (uint32_t)&g_ecc256_on_sce,
#else
  .ecc256_on_sce = 0,
#endif
#if R_SCE_SERVICES_ECC_WRAPPED_192
  .ecc192_on_sce_hrk = (uint32_t)&g_ecc192_on_sce_hrk,
#else
  .ecc192_on_sce_hrk = 0,
#endif
#if R_SCE_SERVICES_ECC_WRAPPED_256
  .ecc256_on_sce_hrk = (uint32_t)&g_ecc256_on_sce_hrk,
#else
  .ecc256_on_sce_hrk = 0,
#endif
#if R_SCE_SERVICES_TRNG
  .trng_on_sce = (uint32_t)&g_trng_on_sce
#else
  .trng_on_sce = 0
#endif
        };

#endif /* Crypto on S1 / SCE5 / SCE7 */

crypto_ctrl_t g_sce_ctrl;
crypto_cfg_t g_sce_cfg =
{ .p_sce_long_plg_start_callback = NULL,
  .p_sce_long_plg_end_callback = NULL,
  .endian_flag = CRYPTO_WORD_ENDIAN_BIG,
  .p_sce_api_interfaces = &g_sce_selected_api_interfaces };
const crypto_instance_t g_sce =
{ .p_ctrl = &g_sce_ctrl, .p_cfg = &g_sce_cfg, .p_api = &g_sce_crypto_api };
trng_ctrl_t g_sce_trng_ctrl;
trng_cfg_t g_sce_trng_cfg =
{ .p_crypto_api = &g_sce_crypto_api, .nattempts = 2 };
const trng_instance_t g_sce_trng =
{ .p_ctrl = &g_sce_trng_ctrl, .p_cfg = &g_sce_trng_cfg, .p_api = &g_trng_on_sce };
const elc_instance_t g_elc =
{ .p_api = &g_elc_on_elc, .p_cfg = NULL };
const ioport_instance_t g_ioport =
{ .p_api = &g_ioport_on_ioport, .p_cfg = NULL };
/* Instance structure to use this module. */
const fmi_instance_t g_fmi =
{ .p_api = &g_fmi_on_fmi };
const cgc_instance_t g_cgc =
{ .p_api = &g_cgc_on_cgc, .p_cfg = NULL };
void g_common_init(void)
{

}
