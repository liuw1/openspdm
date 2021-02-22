/** @file
  Application for Edwards-Curve Key Retrieving (from PEM and X509) & Signature Validation.

Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "Cryptest.h"

//
// Root CA X509 Certificate for X509 Verification Routine (Generated by OpenSSL utility).
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT8 EdTestRootCer[]= {
0x30, 0x82, 0x01, 0x55, 0x30, 0x82, 0x01, 0x07, 0xa0, 0x03, 0x02, 0x01, 0x02, 0x02, 0x14, 0x13, 
0xe2, 0x3d, 0x07, 0xe6, 0xcc, 0xcd, 0x40, 0x6b, 0xf7, 0xc5, 0x57, 0x3a, 0xe5, 0x83, 0xd7, 0x1d, 
0x81, 0x95, 0xe1, 0x30, 0x05, 0x06, 0x03, 0x2b, 0x65, 0x70, 0x30, 0x20, 0x31, 0x1e, 0x30, 0x1c, 
0x06, 0x03, 0x55, 0x04, 0x03, 0x0c, 0x15, 0x69, 0x6e, 0x74, 0x65, 0x6c, 0x20, 0x74, 0x65, 0x73, 
0x74, 0x20, 0x45, 0x44, 0x32, 0x35, 0x35, 0x31, 0x39, 0x20, 0x43, 0x41, 0x30, 0x1e, 0x17, 0x0d, 
0x32, 0x31, 0x30, 0x32, 0x32, 0x32, 0x30, 0x35, 0x34, 0x39, 0x34, 0x36, 0x5a, 0x17, 0x0d, 0x33, 
0x31, 0x30, 0x32, 0x32, 0x30, 0x30, 0x35, 0x34, 0x39, 0x34, 0x36, 0x5a, 0x30, 0x20, 0x31, 0x1e, 
0x30, 0x1c, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c, 0x15, 0x69, 0x6e, 0x74, 0x65, 0x6c, 0x20, 0x74, 
0x65, 0x73, 0x74, 0x20, 0x45, 0x44, 0x32, 0x35, 0x35, 0x31, 0x39, 0x20, 0x43, 0x41, 0x30, 0x2a, 
0x30, 0x05, 0x06, 0x03, 0x2b, 0x65, 0x70, 0x03, 0x21, 0x00, 0x34, 0x35, 0x8c, 0xf3, 0xa3, 0x51, 
0xdc, 0x61, 0x53, 0x5a, 0x2d, 0xee, 0x56, 0x11, 0x2c, 0x6d, 0x27, 0x8b, 0xdd, 0x0b, 0xae, 0xf8, 
0x88, 0xc5, 0x40, 0x38, 0xf3, 0x3a, 0x08, 0x07, 0xa1, 0xd0, 0xa3, 0x53, 0x30, 0x51, 0x30, 0x1d, 
0x06, 0x03, 0x55, 0x1d, 0x0e, 0x04, 0x16, 0x04, 0x14, 0x6e, 0x47, 0x9a, 0xf6, 0x17, 0x24, 0x44, 
0xd9, 0x1d, 0x95, 0xdd, 0x38, 0x61, 0x63, 0xde, 0xa7, 0xea, 0x6a, 0x1c, 0x2c, 0x30, 0x1f, 0x06, 
0x03, 0x55, 0x1d, 0x23, 0x04, 0x18, 0x30, 0x16, 0x80, 0x14, 0x6e, 0x47, 0x9a, 0xf6, 0x17, 0x24, 
0x44, 0xd9, 0x1d, 0x95, 0xdd, 0x38, 0x61, 0x63, 0xde, 0xa7, 0xea, 0x6a, 0x1c, 0x2c, 0x30, 0x0f, 
0x06, 0x03, 0x55, 0x1d, 0x13, 0x01, 0x01, 0xff, 0x04, 0x05, 0x30, 0x03, 0x01, 0x01, 0xff, 0x30, 
0x05, 0x06, 0x03, 0x2b, 0x65, 0x70, 0x03, 0x41, 0x00, 0x59, 0x28, 0x15, 0x28, 0xe3, 0xf2, 0xe6, 
0xca, 0x76, 0x9d, 0x6f, 0x99, 0xd6, 0x29, 0x48, 0xbc, 0x88, 0x9f, 0x73, 0x2f, 0x20, 0x89, 0x27, 
0x30, 0x68, 0xa8, 0x53, 0x95, 0x13, 0x26, 0x4a, 0x6c, 0x5e, 0x4a, 0xec, 0xe1, 0xfb, 0xc6, 0xae, 
0x0f, 0x38, 0x28, 0x40, 0x1f, 0x09, 0x87, 0xe1, 0x3a, 0x46, 0x6f, 0x88, 0x53, 0xba, 0x37, 0xb5, 
0x70, 0xa1, 0xbb, 0x85, 0x35, 0x22, 0xb1, 0xba, 0x0d, };

//
// PEM Key data for Ed Private Key Retrieving.
// (Generated by OpenSSL utility).
//
GLOBAL_REMOVE_IF_UNREFERENCED CONST UINT8 EdTestPemKey[]= {
0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x42, 0x45, 0x47, 0x49, 0x4e, 0x20, 0x50, 0x52, 0x49, 0x56, 0x41, 
0x54, 0x45, 0x20, 0x4b, 0x45, 0x59, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x0a, 0x4d, 0x43, 0x34, 0x43, 
0x41, 0x51, 0x41, 0x77, 0x42, 0x51, 0x59, 0x44, 0x4b, 0x32, 0x56, 0x77, 0x42, 0x43, 0x49, 0x45, 
0x49, 0x49, 0x64, 0x48, 0x79, 0x50, 0x56, 0x45, 0x48, 0x64, 0x32, 0x73, 0x77, 0x6a, 0x70, 0x39, 
0x38, 0x4c, 0x4d, 0x76, 0x61, 0x6e, 0x78, 0x54, 0x4c, 0x6a, 0x6c, 0x74, 0x64, 0x65, 0x4d, 0x6c, 
0x2b, 0x39, 0x78, 0x49, 0x41, 0x2b, 0x38, 0x59, 0x34, 0x54, 0x70, 0x4f, 0x0a, 0x2d, 0x2d, 0x2d, 
0x2d, 0x2d, 0x45, 0x4e, 0x44, 0x20, 0x50, 0x52, 0x49, 0x56, 0x41, 0x54, 0x45, 0x20, 0x4b, 0x45, 
0x59, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x0a, };

/**
  Validate UEFI-OpenSSL Ed Key Retrieving (from PEM & X509) & Signature Interfaces.

  @retval  EFI_SUCCESS  Validation succeeded.
  @retval  EFI_ABORTED  Validation failed.

**/
EFI_STATUS
ValidateCryptEd2 (
  VOID
  )
{
  BOOLEAN Status;
  VOID    *EdPrivKey;
  VOID    *EdPubKey;
  UINT8   Message[] = "EdDsaTest";
  UINT8   Signature[57 * 2];
  UINTN   SigSize;

  Print ("\nUEFI-OpenSSL Ed Key Retrieving Testing: ");

  //
  // Retrieve Ed private key from PEM data.
  //
  Print ("\n- Retrieve Ed Private Key for PEM ...");
  Status = EdGetPrivateKeyFromPem (EdTestPemKey, sizeof (EdTestPemKey), NULL, &EdPrivKey);
  if (!Status) {
    Print ("[Fail]");
    return EFI_ABORTED;
  } else {
    Print ("[Pass]");
  }

  //
  // Retrieve Ed public key from X509 Certificate.
  //
  Print ("\n- Retrieve Ed Public Key from X509 ... ");
  Status    = EdGetPublicKeyFromX509 (EdTestRootCer, sizeof (EdTestRootCer), &EdPubKey);
  if (!Status) {
    Print ("[Fail]");
    EdFree (EdPrivKey);
    return EFI_ABORTED;
  } else {
    Print ("[Pass]");
  }
  
  //
  // Verify Ed-DSA
  //
  SigSize = sizeof(Signature);
  Print ("\n- Ed-DSA Signing ... ");
  Status  = EdDsaSign (EdPrivKey, Message, sizeof(Message), Signature, &SigSize);
  if (!Status) {
    Print ("[Fail]");
    EdFree (EdPrivKey);
    EdFree (EdPubKey);
    return EFI_ABORTED;
  } else {
    Print ("[Pass]");
  }

  Print ("\n- Ed-DSA Verification ... ");
  Status = EdDsaVerify (EdPubKey, Message, sizeof(Message), Signature, SigSize);
  if (!Status) {
    Print ("[Fail]");
    EdFree (EdPrivKey);
    EdFree (EdPubKey);
    return EFI_ABORTED;
  } else {
    Print ("[Pass]\n");
  }

  EdFree (EdPrivKey);
  EdFree (EdPubKey);
  return EFI_SUCCESS;
}