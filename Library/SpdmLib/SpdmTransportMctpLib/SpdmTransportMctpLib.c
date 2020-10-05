/** @file
  SPDM transport library.
  It follows the SPDM Specification.

Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/SpdmTransportMctpLib.h>
#include <IndustryStandard/MctpBinding.h>

#define MCTP_ALIGNMENT 1

RETURN_STATUS
EFIAPI
SpdmMctpEncodeMessage (
  IN     VOID                 *SpdmContext,
  IN     BOOLEAN              IsSecuredMessage,
  IN     UINTN                SpdmMessageSize,
  IN     VOID                 *SpdmMessage,
  IN OUT UINTN                *MctpMessageSize,
     OUT VOID                 *MctpMessage
  )
{
  UINTN                       AlignedSpdmMessageSize;
  UINTN                       Alignment = MCTP_ALIGNMENT;

  AlignedSpdmMessageSize = (SpdmMessageSize + (Alignment - 1)) & ~(Alignment - 1);

  ASSERT (*MctpMessageSize >= AlignedSpdmMessageSize + 1);
  if (*MctpMessageSize < AlignedSpdmMessageSize + 1) {
    *MctpMessageSize = AlignedSpdmMessageSize + 1;
    return RETURN_BUFFER_TOO_SMALL;
  }
  *MctpMessageSize = AlignedSpdmMessageSize + 1;
  if (IsSecuredMessage) {
    *(UINT8 *)MctpMessage = MCTP_MESSAGE_TYPE_SECURED_MCTP;
  } else {
    *(UINT8 *)MctpMessage = MCTP_MESSAGE_TYPE_SPDM;
  }
  CopyMem ((UINT8 *)MctpMessage + 1, SpdmMessage, SpdmMessageSize);
  ZeroMem ((UINT8 *)MctpMessage + 1 + SpdmMessageSize, *MctpMessageSize - 1 - SpdmMessageSize);
  return RETURN_SUCCESS;
}

RETURN_STATUS
EFIAPI
SpdmMctpDecodeMessage (
  IN     VOID                 *SpdmContext,
     OUT BOOLEAN              *IsSecuredMessage,
  IN     UINTN                MctpMessageSize,
  IN     VOID                 *MctpMessage,
  IN OUT UINTN                *SpdmMessageSize,
     OUT VOID                 *SpdmMessage
  )
{
  UINTN                       Alignment = MCTP_ALIGNMENT;

  ASSERT (MctpMessageSize > 1);
  if (MctpMessageSize <= 1) {
    return RETURN_UNSUPPORTED;
  }
  switch (*(UINT8 *)MctpMessage) {
  case MCTP_MESSAGE_TYPE_SECURED_MCTP:
    *IsSecuredMessage = TRUE;
    break;
  case MCTP_MESSAGE_TYPE_SPDM:
    *IsSecuredMessage = FALSE;
    break;
  default:
    return RETURN_UNSUPPORTED;
  }

  ASSERT (((MctpMessageSize - 1) & (Alignment - 1)) == 0);

  if (*SpdmMessageSize < MctpMessageSize - 1) {
    //
    // Handle special case for the side effect of alignment
    // Caller may allocate a good enough buffer without considering alignment.
    // Here we will not copy all the message and ignore the the last padding bytes.
    //
    if (*SpdmMessageSize + Alignment - 1 >= MctpMessageSize - 1) {
      CopyMem (SpdmMessage, (UINT8 *)MctpMessage + 1, *SpdmMessageSize);
      return RETURN_SUCCESS;
    }
    ASSERT (*SpdmMessageSize >= MctpMessageSize - 1);
    *SpdmMessageSize = MctpMessageSize - 1;
    return RETURN_BUFFER_TOO_SMALL;
  }
  *SpdmMessageSize = MctpMessageSize - 1;
  CopyMem (SpdmMessage, (UINT8 *)MctpMessage + 1, *SpdmMessageSize);
  return RETURN_SUCCESS;
}
