/** @file
  HMAC-SHA256 KDF Wrapper Implementation over OpenSSL.

  RFC 5869: HMAC-based Extract-and-Expand Key Derivation Function (HKDF)

Copyright (c) 2018 - 2019, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "InternalCryptLib.h"
#include <openssl/evp.h>
#include <openssl/kdf.h>

/**
  Derive HMAC-based Extract-and-Expand Key Derivation Function (HKDF).

  @param[in]   Md               Message Digest.
  @param[in]   Key              Pointer to the user-supplied key.
  @param[in]   KeySize          Key size in bytes.
  @param[in]   Salt             Pointer to the salt(non-secret) value.
  @param[in]   SaltSize         Salt size in bytes.
  @param[in]   Info             Pointer to the application specific info.
  @param[in]   InfoSize         Info size in bytes.
  @param[out]  Out              Pointer to buffer to receive hkdf value.
  @param[in]   OutSize          Size of hkdf bytes to generate.

  @retval TRUE   Hkdf generated successfully.
  @retval FALSE  Hkdf generation failed.

**/
BOOLEAN
HkdfMdExtractAndExpand (
  IN   CONST EVP_MD *Md,
  IN   CONST UINT8  *Key,
  IN   UINTN        KeySize,
  IN   CONST UINT8  *Salt,
  IN   UINTN        SaltSize,
  IN   CONST UINT8  *Info,
  IN   UINTN        InfoSize,
  OUT  UINT8        *Out,
  IN   UINTN        OutSize
  )
{
  EVP_PKEY_CTX *pHkdfCtx;
  BOOLEAN Result;

  if (Key == NULL || Salt == NULL || Info == NULL || Out == NULL ||
    KeySize > INT_MAX || SaltSize > INT_MAX || InfoSize > INT_MAX || OutSize > INT_MAX ) {
    return FALSE;
  }

  pHkdfCtx = EVP_PKEY_CTX_new_id(EVP_PKEY_HKDF, NULL);
  if (pHkdfCtx == NULL) {
    return FALSE;
  }

  Result = EVP_PKEY_derive_init(pHkdfCtx) > 0;
  if (Result) {
    Result = EVP_PKEY_CTX_set_hkdf_md(pHkdfCtx, Md) > 0;
  }
  if (Result) {
    Result = EVP_PKEY_CTX_set1_hkdf_salt(pHkdfCtx, Salt, (UINT32)SaltSize) > 0;
  }
  if (Result) {
    Result = EVP_PKEY_CTX_set1_hkdf_key(pHkdfCtx, Key, (UINT32)KeySize) > 0;
  }
  if (Result) {
    Result = EVP_PKEY_CTX_add1_hkdf_info(pHkdfCtx, Info, (UINT32)InfoSize) > 0;
  }
  if (Result) {
    Result = EVP_PKEY_derive(pHkdfCtx, Out, &OutSize) > 0;
  }

  EVP_PKEY_CTX_free(pHkdfCtx);
  pHkdfCtx = NULL;
  return Result;
}

/**
  Derive HMAC-based Extract Key Derivation Function (HKDF).

  @param[in]   Md               Message Digest.
  @param[in]   Key              Pointer to the user-supplied key.
  @param[in]   KeySize          Key size in bytes.
  @param[in]   Salt             Pointer to the salt(non-secret) value.
  @param[in]   SaltSize         Salt size in bytes.
  @param[out]  PrkOut           Pointer to buffer to receive hkdf value.
  @param[in]   PrkOutSize       Size of hkdf bytes to generate.

  @retval TRUE   Hkdf generated successfully.
  @retval FALSE  Hkdf generation failed.

**/
BOOLEAN
EFIAPI
HkdfMdExtract (
  IN   CONST EVP_MD *Md,
  IN   CONST UINT8  *Key,
  IN   UINTN        KeySize,
  IN   CONST UINT8  *Salt,
  IN   UINTN        SaltSize,
  OUT  UINT8        *PrkOut,
  IN   UINTN        PrkOutSize
  )
{
  EVP_PKEY_CTX *pHkdfCtx;
  BOOLEAN Result;

  if (Key == NULL || Salt == NULL || PrkOut == NULL ||
    KeySize > INT_MAX || SaltSize > INT_MAX || PrkOutSize > INT_MAX ) {
    return FALSE;
  }

  pHkdfCtx = EVP_PKEY_CTX_new_id(EVP_PKEY_HKDF, NULL);
  if (pHkdfCtx == NULL) {
    return FALSE;
  }

  Result = EVP_PKEY_derive_init(pHkdfCtx) > 0;
  if (Result) {
    Result = EVP_PKEY_CTX_set_hkdf_md(pHkdfCtx, Md) > 0;
  }
  if (Result) {
    Result = EVP_PKEY_CTX_hkdf_mode(pHkdfCtx, EVP_PKEY_HKDEF_MODE_EXTRACT_ONLY) > 0;
  }
  if (Result) {
    Result = EVP_PKEY_CTX_set1_hkdf_salt(pHkdfCtx, Salt, (UINT32)SaltSize) > 0;
  }
  if (Result) {
    Result = EVP_PKEY_CTX_set1_hkdf_key(pHkdfCtx, Key, (UINT32)KeySize) > 0;
  }
  if (Result) {
    Result = EVP_PKEY_derive(pHkdfCtx, PrkOut, &PrkOutSize) > 0;
  }

  EVP_PKEY_CTX_free(pHkdfCtx);
  pHkdfCtx = NULL;
  return Result;
}

/**
  Derive SHA256 HMAC-based Expand Key Derivation Function (HKDF).

  @param[in]   Md               Message Digest.
  @param[in]   Prk              Pointer to the user-supplied key.
  @param[in]   PrkSize          Key size in bytes.
  @param[in]   Info             Pointer to the application specific info.
  @param[in]   InfoSize         Info size in bytes.
  @param[out]  Out              Pointer to buffer to receive hkdf value.
  @param[in]   OutSize          Size of hkdf bytes to generate.

  @retval TRUE   Hkdf generated successfully.
  @retval FALSE  Hkdf generation failed.

**/
BOOLEAN
EFIAPI
HkdfMdExpand (
  IN   CONST EVP_MD *Md,
  IN   CONST UINT8  *Prk,
  IN   UINTN        PrkSize,
  IN   CONST UINT8  *Info,
  IN   UINTN        InfoSize,
  OUT  UINT8        *Out,
  IN   UINTN        OutSize
  )
{
  EVP_PKEY_CTX *pHkdfCtx;
  BOOLEAN Result;

  if (Prk == NULL || Info == NULL || Out == NULL ||
    PrkSize > INT_MAX || InfoSize > INT_MAX || OutSize > INT_MAX ) {
    return FALSE;
  }

  pHkdfCtx = EVP_PKEY_CTX_new_id(EVP_PKEY_HKDF, NULL);
  if (pHkdfCtx == NULL) {
    return FALSE;
  }

  Result = EVP_PKEY_derive_init(pHkdfCtx) > 0;
  if (Result) {
    Result = EVP_PKEY_CTX_set_hkdf_md(pHkdfCtx, Md) > 0;
  }
  if (Result) {
    Result = EVP_PKEY_CTX_hkdf_mode(pHkdfCtx, EVP_PKEY_HKDEF_MODE_EXPAND_ONLY) > 0;
  }
  if (Result) {
    Result = EVP_PKEY_CTX_set1_hkdf_key(pHkdfCtx, Prk, (UINT32)PrkSize) > 0;
  }
  if (Result) {
    Result = EVP_PKEY_CTX_add1_hkdf_info(pHkdfCtx, Info, (UINT32)InfoSize) > 0;
  }
  if (Result) {
    Result = EVP_PKEY_derive(pHkdfCtx, Out, &OutSize) > 0;
  }

  EVP_PKEY_CTX_free(pHkdfCtx);
  pHkdfCtx = NULL;
  return Result;
}

/**
  Derive SHA256 HMAC-based Extract-and-Expand Key Derivation Function (HKDF).

  @param[in]   Key              Pointer to the user-supplied key.
  @param[in]   KeySize          Key size in bytes.
  @param[in]   Salt             Pointer to the salt(non-secret) value.
  @param[in]   SaltSize         Salt size in bytes.
  @param[in]   Info             Pointer to the application specific info.
  @param[in]   InfoSize         Info size in bytes.
  @param[out]  Out              Pointer to buffer to receive hkdf value.
  @param[in]   OutSize          Size of hkdf bytes to generate.

  @retval TRUE   Hkdf generated successfully.
  @retval FALSE  Hkdf generation failed.

**/
BOOLEAN
EFIAPI
HkdfSha256ExtractAndExpand (
  IN   CONST UINT8  *Key,
  IN   UINTN        KeySize,
  IN   CONST UINT8  *Salt,
  IN   UINTN        SaltSize,
  IN   CONST UINT8  *Info,
  IN   UINTN        InfoSize,
  OUT  UINT8        *Out,
  IN   UINTN        OutSize
  )
{
  return HkdfMdExtractAndExpand (EVP_sha256(), Key, KeySize, Salt, SaltSize, Info, InfoSize, Out, OutSize);
}

/**
  Derive SHA256 HMAC-based Extract Key Derivation Function (HKDF).

  @param[in]   Key              Pointer to the user-supplied key.
  @param[in]   KeySize          Key size in bytes.
  @param[in]   Salt             Pointer to the salt(non-secret) value.
  @param[in]   SaltSize         Salt size in bytes.
  @param[out]  PrkOut           Pointer to buffer to receive hkdf value.
  @param[in]   PrkOutSize       Size of hkdf bytes to generate.

  @retval TRUE   Hkdf generated successfully.
  @retval FALSE  Hkdf generation failed.

**/
BOOLEAN
EFIAPI
HkdfSha256Extract (
  IN   CONST UINT8  *Key,
  IN   UINTN        KeySize,
  IN   CONST UINT8  *Salt,
  IN   UINTN        SaltSize,
  OUT  UINT8        *PrkOut,
  IN   UINTN        PrkOutSize
  )
{
  return HkdfMdExtract (EVP_sha256(), Key, KeySize, Salt, SaltSize, PrkOut, PrkOutSize);
}

/**
  Derive SHA256 HMAC-based Expand Key Derivation Function (HKDF).

  @param[in]   Prk              Pointer to the user-supplied key.
  @param[in]   PrkSize          Key size in bytes.
  @param[in]   Info             Pointer to the application specific info.
  @param[in]   InfoSize         Info size in bytes.
  @param[out]  Out              Pointer to buffer to receive hkdf value.
  @param[in]   OutSize          Size of hkdf bytes to generate.

  @retval TRUE   Hkdf generated successfully.
  @retval FALSE  Hkdf generation failed.

**/
BOOLEAN
EFIAPI
HkdfSha256Expand (
  IN   CONST UINT8  *Prk,
  IN   UINTN        PrkSize,
  IN   CONST UINT8  *Info,
  IN   UINTN        InfoSize,
  OUT  UINT8        *Out,
  IN   UINTN        OutSize
  )
{
  return HkdfMdExpand (EVP_sha256(), Prk, PrkSize, Info, InfoSize, Out, OutSize);
}

/**
  Derive SHA384 HMAC-based Extract-and-Expand Key Derivation Function (HKDF).

  @param[in]   Key              Pointer to the user-supplied key.
  @param[in]   KeySize          Key size in bytes.
  @param[in]   Salt             Pointer to the salt(non-secret) value.
  @param[in]   SaltSize         Salt size in bytes.
  @param[in]   Info             Pointer to the application specific info.
  @param[in]   InfoSize         Info size in bytes.
  @param[out]  Out              Pointer to buffer to receive hkdf value.
  @param[in]   OutSize          Size of hkdf bytes to generate.

  @retval TRUE   Hkdf generated successfully.
  @retval FALSE  Hkdf generation failed.

**/
BOOLEAN
EFIAPI
HkdfSha384ExtractAndExpand (
  IN   CONST UINT8  *Key,
  IN   UINTN        KeySize,
  IN   CONST UINT8  *Salt,
  IN   UINTN        SaltSize,
  IN   CONST UINT8  *Info,
  IN   UINTN        InfoSize,
  OUT  UINT8        *Out,
  IN   UINTN        OutSize
  )
{
  return HkdfMdExtractAndExpand (EVP_sha384(), Key, KeySize, Salt, SaltSize, Info, InfoSize, Out, OutSize);
}

/**
  Derive SHA384 HMAC-based Extract Key Derivation Function (HKDF).

  @param[in]   Key              Pointer to the user-supplied key.
  @param[in]   KeySize          Key size in bytes.
  @param[in]   Salt             Pointer to the salt(non-secret) value.
  @param[in]   SaltSize         Salt size in bytes.
  @param[out]  PrkOut           Pointer to buffer to receive hkdf value.
  @param[in]   PrkOutSize       Size of hkdf bytes to generate.

  @retval TRUE   Hkdf generated successfully.
  @retval FALSE  Hkdf generation failed.

**/
BOOLEAN
EFIAPI
HkdfSha384Extract (
  IN   CONST UINT8  *Key,
  IN   UINTN        KeySize,
  IN   CONST UINT8  *Salt,
  IN   UINTN        SaltSize,
  OUT  UINT8        *PrkOut,
  IN   UINTN        PrkOutSize
  )
{
  return HkdfMdExtract (EVP_sha384(), Key, KeySize, Salt, SaltSize, PrkOut, PrkOutSize);
}

/**
  Derive SHA384 HMAC-based Expand Key Derivation Function (HKDF).

  @param[in]   Prk              Pointer to the user-supplied key.
  @param[in]   PrkSize          Key size in bytes.
  @param[in]   Info             Pointer to the application specific info.
  @param[in]   InfoSize         Info size in bytes.
  @param[out]  Out              Pointer to buffer to receive hkdf value.
  @param[in]   OutSize          Size of hkdf bytes to generate.

  @retval TRUE   Hkdf generated successfully.
  @retval FALSE  Hkdf generation failed.

**/
BOOLEAN
EFIAPI
HkdfSha384Expand (
  IN   CONST UINT8  *Prk,
  IN   UINTN        PrkSize,
  IN   CONST UINT8  *Info,
  IN   UINTN        InfoSize,
  OUT  UINT8        *Out,
  IN   UINTN        OutSize
  )
{
  return HkdfMdExpand (EVP_sha384(), Prk, PrkSize, Info, InfoSize, Out, OutSize);
}

/**
  Derive SHA512 HMAC-based Extract-and-Expand Key Derivation Function (HKDF).

  @param[in]   Key              Pointer to the user-supplied key.
  @param[in]   KeySize          Key size in bytes.
  @param[in]   Salt             Pointer to the salt(non-secret) value.
  @param[in]   SaltSize         Salt size in bytes.
  @param[in]   Info             Pointer to the application specific info.
  @param[in]   InfoSize         Info size in bytes.
  @param[out]  Out              Pointer to buffer to receive hkdf value.
  @param[in]   OutSize          Size of hkdf bytes to generate.

  @retval TRUE   Hkdf generated successfully.
  @retval FALSE  Hkdf generation failed.

**/
BOOLEAN
EFIAPI
HkdfSha512ExtractAndExpand (
  IN   CONST UINT8  *Key,
  IN   UINTN        KeySize,
  IN   CONST UINT8  *Salt,
  IN   UINTN        SaltSize,
  IN   CONST UINT8  *Info,
  IN   UINTN        InfoSize,
  OUT  UINT8        *Out,
  IN   UINTN        OutSize
  )
{
  return HkdfMdExtractAndExpand (EVP_sha512(), Key, KeySize, Salt, SaltSize, Info, InfoSize, Out, OutSize);
}

/**
  Derive SHA512 HMAC-based Extract Key Derivation Function (HKDF).

  @param[in]   Key              Pointer to the user-supplied key.
  @param[in]   KeySize          Key size in bytes.
  @param[in]   Salt             Pointer to the salt(non-secret) value.
  @param[in]   SaltSize         Salt size in bytes.
  @param[out]  PrkOut           Pointer to buffer to receive hkdf value.
  @param[in]   PrkOutSize       Size of hkdf bytes to generate.

  @retval TRUE   Hkdf generated successfully.
  @retval FALSE  Hkdf generation failed.

**/
BOOLEAN
EFIAPI
HkdfSha512Extract (
  IN   CONST UINT8  *Key,
  IN   UINTN        KeySize,
  IN   CONST UINT8  *Salt,
  IN   UINTN        SaltSize,
  OUT  UINT8        *PrkOut,
  IN   UINTN        PrkOutSize
  )
{
  return HkdfMdExtract (EVP_sha512(), Key, KeySize, Salt, SaltSize, PrkOut, PrkOutSize);
}

/**
  Derive SHA512 HMAC-based Expand Key Derivation Function (HKDF).

  @param[in]   Prk              Pointer to the user-supplied key.
  @param[in]   PrkSize          Key size in bytes.
  @param[in]   Info             Pointer to the application specific info.
  @param[in]   InfoSize         Info size in bytes.
  @param[out]  Out              Pointer to buffer to receive hkdf value.
  @param[in]   OutSize          Size of hkdf bytes to generate.

  @retval TRUE   Hkdf generated successfully.
  @retval FALSE  Hkdf generation failed.

**/
BOOLEAN
EFIAPI
HkdfSha512Expand (
  IN   CONST UINT8  *Prk,
  IN   UINTN        PrkSize,
  IN   CONST UINT8  *Info,
  IN   UINTN        InfoSize,
  OUT  UINT8        *Out,
  IN   UINTN        OutSize
  )
{
  return HkdfMdExpand (EVP_sha512(), Prk, PrkSize, Info, InfoSize, Out, OutSize);
}
