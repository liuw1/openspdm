/** @file
  SPDM common library.
  It follows the SPDM Specification.

Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "SpdmRequesterLibInternal.h"

#pragma pack(1)

typedef struct {
  SPDM_MESSAGE_HEADER  Header;
  UINT8                VerifyData[MAX_HASH_SIZE];
} SPDM_PSK_FINISH_REQUEST_MINE;

#pragma pack()

/**
  This function sends PSK_FINISH and receives PSK_FINISH_RSP for SPDM PSK finish.

  @param  SpdmContext                  A pointer to the SPDM context.
  @param  SessionId                    SessionId to the PSK_FINISH request.

  @retval RETURN_SUCCESS               The PSK_FINISH is sent and the PSK_FINISH_RSP is received.
  @retval RETURN_DEVICE_ERROR          A device error occurs when communicates with the device.
**/
RETURN_STATUS
SpdmSendReceivePskFinish (
  IN     SPDM_DEVICE_CONTEXT  *SpdmContext,
  IN     UINT32               SessionId
  )
{
  RETURN_STATUS                             Status;
  SPDM_PSK_FINISH_REQUEST_MINE              SpdmRequest;
  UINTN                                     SpdmRequestSize;
  UINTN                                     HmacSize;
  SPDM_PSK_FINISH_RESPONSE                  SpdmResponse;
  UINTN                                     SpdmResponseSize;
  SPDM_SESSION_INFO                         *SessionInfo;
  UINT8                                     TH2HashData[64];
  
  if ((SpdmContext->ConnectionInfo.Capability.Flags & SPDM_GET_CAPABILITIES_RESPONSE_FLAGS_PSK_CAP) == 0) {
    return RETURN_DEVICE_ERROR;
  }

  SessionInfo = SpdmGetSessionInfoViaSessionId (SpdmContext, SessionId);
  if (SessionInfo == NULL) {
    ASSERT (FALSE);
    return RETURN_UNSUPPORTED;
  }
  
  SpdmContext->ErrorState = SPDM_STATUS_ERROR_DEVICE_NO_CAPABILITIES;
   
  SpdmRequest.Header.SPDMVersion = SPDM_MESSAGE_VERSION_11;
  SpdmRequest.Header.RequestResponseCode = SPDM_PSK_FINISH;
  SpdmRequest.Header.Param1 = 0;
  SpdmRequest.Header.Param2 = 0;
  
  HmacSize = GetSpdmHashSize (SpdmContext->ConnectionInfo.Algorithm.BaseHashAlgo);
  SpdmRequestSize = sizeof(SPDM_FINISH_REQUEST) + HmacSize;
  
  AppendManagedBuffer (&SessionInfo->SessionTranscript.MessageF, (UINT8 *)&SpdmRequest, SpdmRequestSize - HmacSize);

  SpdmGeneratePskFinishReqHmac (SpdmContext, SessionInfo, SpdmRequest.VerifyData);

  AppendManagedBuffer (&SessionInfo->SessionTranscript.MessageF, (UINT8 *)&SpdmRequest + SpdmRequestSize - HmacSize, HmacSize);

  Status = SpdmSendSpdmRequest (SpdmContext, &SessionId, SpdmRequestSize, &SpdmRequest);
  if (RETURN_ERROR(Status)) {
    return RETURN_DEVICE_ERROR;
  }

  SpdmResponseSize = sizeof(SpdmResponse);
  ZeroMem (&SpdmResponse, sizeof(SpdmResponse));
  Status = SpdmReceiveSpdmResponse (SpdmContext, &SessionId, &SpdmResponseSize, &SpdmResponse);
  if (RETURN_ERROR(Status)) {
    return RETURN_DEVICE_ERROR;
  }
  if (SpdmResponseSize != sizeof(SPDM_PSK_FINISH_RESPONSE)) {
    return RETURN_DEVICE_ERROR;
  }
  if (SpdmResponse.Header.RequestResponseCode != SPDM_PSK_FINISH_RSP) {
    return RETURN_DEVICE_ERROR;
  }
  
  AppendManagedBuffer (&SessionInfo->SessionTranscript.MessageF, &SpdmResponse, SpdmResponseSize);

  DEBUG ((DEBUG_INFO, "SpdmGenerateSessionDataKey[%x]\n", SessionId));
  SpdmCalculateTh2 (SpdmContext, SessionId, TRUE, TH2HashData);
  SpdmGenerateSessionDataKey (SessionInfo->SecuredMessageContext, TH2HashData);

  SpdmSecuredMessageSetSessionState (SessionInfo->SecuredMessageContext, SpdmSessionStateEstablished);
  SpdmContext->ErrorState = SPDM_STATUS_SUCCESS;
  
  return RETURN_SUCCESS;
}

