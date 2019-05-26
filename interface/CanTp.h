/** * @file CanTp.h * @author Guillaume Sottas * @date 15/01/2018 * * @defgroup CANTP CAN transport layer * * @brief CanTp is the module between the PDU router and the CAN interface module. the main purpose of the CAN TP module is to segment and reassemble CAN I-PDUs longer than 8 bytes or longer than 64 bytes in case of CAN FD. the PDU router deploys AUTOSAR COM and DCM I-PDUs onto different communication protocols. the routing through a network system type (e.g. CAN, LIN and FlexRay) depends on the I-PDU identifier. the PDU router also determines if a transport protocol has to be used or not. lastly, this module carries out gateway functionality, when there is no rate conversion. CAN interface (CanIf) provides equal mechanisms to access a CAN bus channel regardless of its location (μC internal/external). from the location of CAN controllers (on chip / onboard), it extracts the ECU hardware layout and the number of CAN drivers. because CanTp only handles transport protocol frames (i.e. SF, FF, CF and FC PDUs), depending on the N-PDU ID, the CAN interface has to forward an I-PDU to CanTp or PduR. * * @brief <b>acronyms:</b> * - CAN L-SDU: this is the SDU of the CAN interface module. it is similar to CAN N-PDU but from the CAN interface module point of view. * - CAN LSduId: this is the unique identifier of a SDU within the CAN interface. it is used for referencing L-SDU’s routing properties. consequently, in order to interact with the CAN interface through its API, an upper layer uses CAN LSduId to refer to a CAN L-SDU info structure. * - CAN N-PDU: this is the PDU of the CAN transport layer. it contains a unique identifier, data length and data (protocol control information plus the whole N-SDU or a part of it). * - CAN N-SDU: this is the SDU of the CAN transport layer. in the AUTOSAR architecture, it is a set of data coming from the PDU router. * - CAN N-SDU info structure: this is a CAN transport layer internal constant structure that contains specific CAN transport layer information to process transmission, reception, segmentation and reassembly of the related CAN N-SDU. * - CAN NSduId: unique SDU identifier within the CAN transport layer. it is used to reference N-SDU’s routing properties. consequently, to interact with the CAN transport layer via its API, an upper layer uses CAN NSduId to refer to a CAN N-SDU info structure. * - I-PDU: this is the PDU of the AUTOSAR COM module. * - PDU: in layered systems, it refers to a data unit that is specified in the protocol of a given layer. this contains user data of that layer (SDU) plus possible protocol control information. furthermore, the PDU of layer X is the SDU of its lower layer X-1 (i.e. (X)-PDU = (X-1)-SDU). * - PduInfoType: this type refers to a structure used to store basic information to process the transmissioneception of a PDU (or a SDU), namely a pointer to its payload in RAM and the corresponding length (in bytes). * - SDU: in layered systems, this refers to a set of data that is sent by a user of the services of a given layer, and is transmitted to a peer service user, whilst remaining semantically unchanged. * * @brief <b>abbreviations</b>: * - \b BS: block size * - \b Can: CAN driver module * - \b CAN CF: CAN consecutive frame N-PDU * - \b CAN FC: CAN flow control N-PDU * - \b CAN FF: CAN first frame N-PDU * - \b CAN SF: CAN single frame N-PDU * - \b CanIf: CAN interface * - \b CanTp: CAN transport layer * - \b CanTrcv: CAN transceiver module * - \b CF: see CAN CF * - \b Com: AUTOSAR COM module * - \b Dcm: diagnostic communication manager module * - \b DEM: diagnostic event manager * - \b DET: default error tracer * - \b DLC: data length code (part of CAN PDU that describes the SDU length) * - \b FC: see CAN FC * - \b FF: see CAN FF * - \b FIM: function inhibition manager * - \b Mtype: message type (possible value: diagnostics, remote diagnostics) * - \b N_AI: network address information (see ISO 15765-2). * - \b N_Ar: time for transmission of the CAN frame (any N-PDU) on the receiver side (see ISO 15765-2). * - \b N_As: time for transmission of the CAN frame (any N-PDU) on the sender side (see ISO 15765-2). * - \b N_Br: time until transmission of the next flow control N-PDU (see ISO 15765-2). * - \b N_Bs: time until reception of the next flow control N-PDU (see ISO 15765-2). * - \b N_Cr: time until reception of the next consecutive frame N-PDU (see ISO 15765-2). * - \b N_Cs: time until transmission of the next consecutive frame N-PDU (see ISO 15765-2). * - \b N_Data: data information of the transport layer * - \b N_PCI: protocol control information of the transport layer * - \b N_SA: network source address (see ISO 15765-2). * - \b N_TA: network target address (see ISO 15765-2). it might already contain the N_TAtype(physical/function) in case of extended addressing. * - \b N_TAtype: network target address type (see ISO 15765-2). * - \b OBD: on-board diagnostic * - \b PDU: protocol data unit * - \b PduR: PDU router * - \b SDU: service data unit * - \b FS: flow Status * - \b CAN FD: CAN flexible data rate * - \b CAN_DL: CAN frame data length * - \b TX_DL: transmit data link layer data length * - \b RX_DL: received data link layer data length * - \b SF_DL: single frame data length in bytes * * @brief dependencies to other modules: * * @defgroup CANTP_H interface * @ingroup CANTP * * @defgroup CANTP_H_GDEF identification informations * @ingroup CANTP_H * @defgroup CANTP_H_E errors classification * @ingroup CANTP_H * @defgroup CANTP_H_E_D development errors * @ingroup CANTP_H_E * @defgroup CANTP_H_E_R runtime errors * @ingroup CANTP_H_E * @defgroup CANTP_H_E_T transient faults * @ingroup CANTP_H_E * @defgroup CANTP_H_GTDEF global data type definitions * @ingroup CANTP_H * @defgroup CANTP_H_EFDECL external function declarations * @ingroup CANTP_H * @defgroup CANTP_H_GCDECL global constant declarations * @ingroup CANTP_H * @defgroup CANTP_H_GVDECL global variable declarations * @ingroup CANTP_H * @defgroup CANTP_H_GFDECL global function declarations * @ingroup CANTP_H * @defgroup CANTP_H_GSFDECL global scheduled function declarations * @ingroup CANTP_H */#ifndef CANTP_H#define CANTP_H#ifdef __cplusplusextern "C"{#endif /* ifdef __cplusplus *//*-----------------------------------------------------------------------------------------------*//* included files (#include).                                                                    *//*-----------------------------------------------------------------------------------------------*//** * @addtogroup CANTP_H * @{ */#include "ComStack_Types.h"#ifdef CANTP_BUILD_CFFI_INTERFACE/** * @brief if CANTP_BUILD_CFFI_INTERFACE is defined, expose the CanTp callback function to CFFI * module. */#include "CanTp_Cbk.h"#endif /* #ifdef CANTP_BUILD_CFFI_INTERFACE *//** @} *//*-----------------------------------------------------------------------------------------------*//* global definitions (#define).                                                                 *//*-----------------------------------------------------------------------------------------------*//** * @addtogroup CANTP_H_GDEF * @{ *//** * @brief unique identifier of the CAN transport layer. * @note this value corresponds to document ID of corresponding Autosar software specification. */#define CANTP_MODULE_ID (0x0Eu)#ifndef CANTP_SW_MAJOR_VERSION/** * @brief CAN transport layer major version number. */#define CANTP_SW_MAJOR_VERSION 0x00u#endif /* #ifndef CANTP_SW_MAJOR_VERSION */#ifndef CANTP_SW_MINOR_VERSION/** * @brief CAN transport layer minor version number. */#define CANTP_SW_MINOR_VERSION 0x01u#endif /* #ifndef CANTP_SW_MINOR_VERSION */#ifndef CANTP_SW_PATCH_VERSION/** * @brief CAN transport layer patch version number. */#define CANTP_SW_PATCH_VERSION 0x00u#endif /* #ifndef CANTP_SW_PATCH_VERSION *//** * @brief @ref CanTp_Init API ID. */#define CANTP_INIT_API_ID (0x01u)/** * @brief @ref CanTp_GetVersionInfo API ID. */#define CANTP_GET_VERSION_INFO_API_ID (0x07u)/** * @brief @ref CanTp_Shutdown API ID. */#define CANTP_SHUTDOWN_API_ID (0x02u)/** * @brief @ref CanTp_Transmit API ID. */#define CANTP_TRANSMIT_API_ID (0x49u)/** * @brief @ref CanTp_CancelTransmit API ID. */#define CANTP_CANCEL_TRANSMIT_API_ID (0x4Au)/** * @brief @ref CanTp_CancelReceive API ID. */#define CANTP_CANCEL_RECEIVE_API_ID (0x4Cu)/** * @brief @ref CanTp_ChangeParameter API ID. */#define CANTP_CHANGE_PARAMETER_API_ID (0x4Bu)/** * @brief @ref CanTp_ReadParameter API ID. */#define CANTP_READ_PARAMETER_API_ID (0x0Bu)/** * @brief @ref CanTp_RxIndication API ID. */#define CANTP_RX_INDICATION_API_ID (0x42u)/** * @brief @ref CanTp_MainFunction API ID. */#define CANTP_MAIN_FUNCTION_API_ID (0x06u)/** @} *//** * @addtogroup CANTP_H_E_D * @{ *//** * @brief API service called with wrong parameter(s): when @ref CanTp_Transmit is called for a none configured PDU identifier or with an identifier for a received PDU. * @satisfy{@req{SWS_CanTp_00293}} */#define CANTP_E_PARAM_CONFIG (0x01u)/** * @brief API service called with wrong parameter(s): when @ref CanTp_Transmit is called for a none configured PDU identifier or with an identifier for a received PDU. * @satisfy{@req{SWS_CanTp_00293}} */#define CANTP_E_PARAM_ID (0x02u)/** * @brief API service called with a NULL pointer. in case of this error, the API service shall return immediately without any further action, besides reporting this development error. * @satisfy{@req{SWS_CanTp_00293}} */#define CANTP_E_PARAM_POINTER (0x03u)/** * @brief module initialization has failed, e.g. @ref CanTp_Init called with an invalid pointer in post-build. * @satisfy{@req{SWS_CanTp_00293}} */#define CANTP_E_INIT_FAILED (0x04u)/** * @brief API service used without module initialization: on any API call except @ref CanTp_Init and @ref CanTp_GetVersionInfo if CanTp is in state CANTP_OFF. * @satisfy{@req{SWS_CanTp_00293}} */#define CANTP_E_UNINIT (0x20u)/** * @brief invalid transmit PDU identifier (e.g. a service is called with an inexistent tx PDU identifier). * @satisfy{@req{SWS_CanTp_00293}} */#define CANTP_E_INVALID_TX_ID (0x30u)/** * @brief invalid receive PDU identifier (e.g. a service is called with an inexistent rx PDU identifier). * @satisfy{@req{SWS_CanTp_00293}} */#define CANTP_E_INVALID_RX_ID (0x40u)/** @} *//** * @addtogroup CANTP_H_E_R * @{ *//** * @brief PDU received with a length smaller than 8 bytes (i.e. PduInfoPtr.SduLength < 8). * @satisfy{@req{SWS_CanTp_00352}} */#define CANTP_E_PADDING (0x70u)/** * @brief @ref CanTp_Transmit is called for a configured tx I-Pdu with functional addressing and the length parameter indicates, that the message can not be sent with a SF. * @satisfy{@req{SWS_CanTp_00352}} */#define CANTP_E_INVALID_TATYPE (0x90u)/** * @brief requested operation is not supported – a cancel transmission/reception request for an N-SDU that it is not on transmission/reception process. * @satisfy{@req{SWS_CanTp_00352}} */#define CANTP_E_OPER_NOT_SUPPORTED (0xA0u)/** * @brief event reported in case of an implementation specific error other than a protocol timeout error during a reception or a transmission. * @satisfy{@req{SWS_CanTp_00352}} */#define CANTP_E_COM (0xB0u)/** * @brief event reported in case of a protocol timeout error during reception. * @satisfy{@req{SWS_CanTp_00352}} */#define CANTP_E_RX_COM (0xC0u)/** * @brief event reported in case of a protocol timeout error during transmission. * @satisfy{@req{SWS_CanTp_00352}} */#define CANTP_E_TX_COM (0xD0u)#define CANTP_I_NONE (0xFFu)#define CANTP_I_N_AS (0x00u)#define CANTP_I_N_BS (0x01u)#define CANTP_I_N_CS (0x02u)#define CANTP_I_N_AR (0x03u)#define CANTP_I_N_BR (0x04u)#define CANTP_I_N_CR (0x05u)#define CANTP_I_ST_MIN (0x01u << 0x06u)#define CANTP_I_FULL_DUPLEX_RX_SF (0x85u)#define CANTP_I_FULL_DUPLEX_RX_FF (0x86u)#define CANTP_I_FULL_DUPLEX_RX_CF (0x87u)#define CANTP_I_FULL_DUPLEX_RX_FC (0x88u)#define CANTP_I_FULL_DUPLEX_RX_UNDEF (0x89u)#define CANTP_I_N_BUFFER_OVFLW (0x90u)#define CANTP_E_UNEXP_PDU (0x85u)/** @} *//** * @addtogroup CANTP_H_E_T * @{ *//** @} *//*-----------------------------------------------------------------------------------------------*//* global data type definitions (typedef, struct).                                               *//*-----------------------------------------------------------------------------------------------*//** * @addtogroup CANTP_H_GTDEF * @{ */typedef enum{    /**     * @brief full duplex channel     */    CANTP_MODE_FULL_DUPLEX,    /**     * @brief half duplex channel     */    CANTP_MODE_HALF_DUPLEX} CanTp_ChannelMode; /* CanTpChannelMode. */typedef enum{    /**     * @brief extended addressing format     */    CANTP_EXTENDED,    /**     * @brief mixed 11 bit addressing format     */    CANTP_MIXED,    /**     * @brief mixed 29 bit addressing format     */    CANTP_MIXED29BIT,    /**     * @brief normal fixed addressing format     */    CANTP_NORMALFIXED,    /**     * @brief normal addressing format     */    CANTP_STANDARD} CanTp_AddressingFormatType;typedef enum{    /**     * @brief padding is used     */    CANTP_OFF,    /**     * @brief padding is not used     */    CANTP_ON} CanTp_StateType;typedef CanTp_StateType CanTp_RxPaddingActivationType;typedef CanTp_StateType CanTp_TxPaddingActivationType;typedef enum{    /**     * @brief functional request type     */    CANTP_FUNCTIONAL,    /**     * @brief physical request type     */    CANTP_PHYSICAL} CanTp_ComTypeType;typedef struct{    uint8 nSa; /* CanTpNSa. */} CanTp_NSaType; /* CanTpNSa. */typedef struct{    uint8 nTa; /* CanTpNTa. */} CanTp_NTaType; /* CanTpNTa. */typedef struct{    uint8 nAe; /* CanTpNAe. */} CanTp_NAeType; /* CanTpNAe. */typedef struct{    const uint32 rxFcNPduRef; /* CanTpRxFcNPduRef. */    const uint16 rxFcNPduId; /* CanTpRxFcNPduId. */} CanTp_RxFcNPduType; /* CanTpRxFcNPdu. */typedef struct{    /**     * @brief reference to a Pdu in the COM stack     */    const uint32 fcNPduRef; /* CanTpTxFcNPduRef. */    /**     * @brief used for grouping of the ID of a PDU and the reference to a PDU. this N-PDU produces     * a meta data item of type CAN_ID_32     */    const uint16 fcNPduId; /* CanTpTxFcNPduConfirmationPduId. */} CanTp_TxFcNPduType; /* CanTpTxFcNPdu. */typedef struct{    /**     * @brief unique identifier to a structure that contains all useful information to process the     * reception of a network SDU.     *     * @warning this identifier must be unique in the scope of CanTp configuration module (both     * directions).     */    const uint16 nSduId; /* CanTpRxNSduId. */    const CanTp_NSaType *pNSa;    const CanTp_NTaType *pNTa;    const CanTp_NAeType *pNAe;    const uint8 bs; /* CanTpBs. */    const uint32 nar; /* CanTpNar. */    const uint32 nbr; /* CanTpNbr. */    const uint32 ncr; /* CanTpNcr. */    const uint16 wftMax; /* CanTpRxWftMax. */    /**     * @brief separation time minimum between two consecutive frames [μs].     */    const uint16 sTMin; /* CanTpSTmin. */    const CanTp_AddressingFormatType af; /* CanTpRxAddressingFormat. */    const CanTp_RxPaddingActivationType padding; /* CanTpRxPaddingActivation. */    const CanTp_ComTypeType taType; /* CanTpRxTaType. */    const PduIdType rxNSduRef; /* CanTpRxNSduRef. */} CanTp_RxNSduType; /* CanTpRxNSdu. */typedef struct{    /**     * @brief unique identifier to a structure that contains all useful information to process the     * transmission of a network SDU.     *     * @warning this identifier must be unique in the scope of CanTp configuration module (both     * directions).     */    const uint16 nSduId; /* CanTpTxNSduId. */    const CanTp_NSaType *pNSa;    const CanTp_NTaType *pNTa;    const CanTp_NAeType *pNAe;    /**     * @brief value [μs] of the N_As timeout. N_As is the time for transmission of a CAN frame (any     * N-PDU) on the part of the sender     */    const uint32 nas; /* CanTpNas. */    /**     * @brief value [μs] of the N_Bs timeout. N_Bs is the time of transmission until reception of     * the next flow control N-PDU     */    const uint32 nbs; /* CanTpNbs. */    /**     * @brief value [μs] of the performance requirement of (N_Cs + N_As). N_Cs is the time in which     * CanTp is allowed to request the tx data of a consecutive frame N-PDU     */    const uint32 ncs; /* CanTpNcs. */    /**     * @brief switch for enabling transmit cancellation     */    const boolean tc; /* CanTpTc. */    /**     * @brief declares which communication addressing format is supported for this TxNSdu     */    const CanTp_AddressingFormatType af; /* CanTpTxAddressingFormat. */    /**     * @brief defines if the transmit frame use padding or not. this parameter is restricted to 8     * byte N-PDUs.     */    const CanTp_TxPaddingActivationType padding; /* CanTpTxPaddingActivation. */    /**     * @brief declares the communication type of this TxNSdu     */    const CanTp_ComTypeType taType; /* CanTpTxTaType. */    /**     * @brief reference to a PDU in the COM stack     */    const uint32 txNSduRef; /* CanTpTxNSduRef. */} CanTp_TxNSduType; /* CanTpTxNSdu. */typedef struct{    const uint16 id; /* CanTpRxNPduId. */    const uint32 rxNPduId; /* CanTpRxNPduRef. */} CanTp_RxNPduType; /* CanTpRxNPdu. */typedef struct{    const uint32 pduRef; /* CanTpTxNPduRef. */    const uint16 pduConfirmationPduId; /* CanTpTxNPduConfirmationPduId. */} CanTp_TxNPduType; /* CanTpTxNPdu. */typedef struct{    struct    {        const CanTp_RxNSduType *rx;        const uint32 rxNSduCnt;        const CanTp_TxNSduType *tx;        const uint32 txNSduCnt;    } nSdu;    const CanTp_ChannelMode channelMode;} CanTp_ChannelType; /* CanTpChannel. *///typedef struct//{    //const boolean changeParamApi; /* CanTpChangeParameterApi. */    //const boolean readParamApi; /* CanTpReadParameterApi. */    //const boolean getVersionApi; /* CanTpVersionInfoApi. */    //const boolean errorDetect; /* CanTpDevErrorDetect. */    //const boolean dynamicId; /* CanTpDynIdSupport. */    //const boolean flexibleDataRate; /* CanTpFlexibleDataRateSupport. */    //const boolean genericConnection; /* CanTpGenericConnectionSupport. */    //const uint8 paddingByte; /* CanTpPaddingByte. *///} CanTp_GeneralType;/** * @brief data structure type for the post-build configuration parameters. * @satisfy{@req{SWS_CanTp_00340}} */typedef struct{    /**     * @brief period between two calls of CanTp_MainFunction [μs].     * @note see CanTpMainFunctionPeriod parameter     */    const uint32 mainFunctionPeriod;    const uint32 maxChannelCnt; /* CanTpMaxChannelCnt. */    const CanTp_ChannelType *pChannel;    const uint8 paddingByte; /* CanTpPaddingByte. */} CanTp_ConfigType;/** @} *//*-----------------------------------------------------------------------------------------------*//* external function declarations (extern).                                                      *//*-----------------------------------------------------------------------------------------------*//** * @addtogroup CANTP_H_EFDECL * @{ *//** @} *//*-----------------------------------------------------------------------------------------------*//* global constant declarations (extern const).                                                  *//*-----------------------------------------------------------------------------------------------*//** * @addtogroup CANTP_H_GCDECL * @{ */#ifdef CANTP_BUILD_CFFI_INTERFACE/** * @brief if CANTP_BUILD_CFFI_INTERFACE is defined, expose the CanTp static Configuration structure * to CFFI module. */#include "CanTp_Cfg.h"#endif /* #ifdef CANTP_BUILD_CFFI_INTERFACE *//** @} *//*-----------------------------------------------------------------------------------------------*//* global variable declarations (extern).                                                        *//*-----------------------------------------------------------------------------------------------*//** * @addtogroup CANTP_H_GVDECL * @{ */extern CanTp_StateType CanTp_State;/** @} *//*-----------------------------------------------------------------------------------------------*//* global function declarations.                                                                 *//*-----------------------------------------------------------------------------------------------*//** * @addtogroup CANTP_H_GFDECL * @{ */#define CanTp_START_SEC_CODE_SLOW#include "CanTp_MemMap.h"/** * @brief this function initializes the CanTp module. * @param [in] pConfig pointer to the CanTp post-build configuration data * @satisfy{@req{SWS_CanTp_00208}} * @satisfy{@req{SWS_CanTp_00199}} */void CanTp_Init(const CanTp_ConfigType *pConfig);#define CanTp_STOP_SEC_CODE_SLOW#include "CanTp_MemMap.h"#define CanTp_START_SEC_CODE_SLOW#include "CanTp_MemMap.h"/** * @brief this function returns the version information of the CanTp module. * @param [out] pVersionInfo indicator as to where to store the version information of this module * @satisfy{@req{SWS_CanTp_00210}} * @satisfy{@req{SWS_CanTp_00319}} */void CanTp_GetVersionInfo(Std_VersionInfoType *pVersionInfo);#define CanTp_STOP_SEC_CODE_SLOW#include "CanTp_MemMap.h"#define CanTp_START_SEC_CODE_FAST#include "CanTp_MemMap.h"/** * @brief this function is called to shutdown the CanTp module. * @satisfy{@req{SWS_CanTp_00211}} * @satisfy{@req{SWS_CanTp_00202}} * @satisfy{@req{SWS_CanTp_00200}} */void CanTp_Shutdown(void);#define CanTp_STOP_SEC_CODE_FAST#include "CanTp_MemMap.h"#define CanTp_START_SEC_CODE_FAST#include "CanTp_MemMap.h"/** * @brief requests transmission of a PDU. * @param [in] txPduId identifier of the PDU to be transmitted * @param [in] pPduInfo length of and pointer to the PDU data and pointer to meta-data * * @retval E_OK transmit request has been accepted * @retval E_NOT_OK transmit request has not been accepted * * @satisfy{@req{SWS_CanTp_00212}} * @satisfy{@req{SWS_CanTp_00231}} * @satisfy{@req{SWS_CanTp_00232}} * @satisfy{@req{SWS_CanTp_00354}} * @satisfy{@req{SWS_CanTp_00204}} * @satisfy{@req{SWS_CanTp_00205}} * @satisfy{@req{SWS_CanTp_00206}} * @satisfy{@req{SWS_CanTp_00298}} * @satisfy{@req{SWS_CanTp_00299}} * @satisfy{@req{SWS_CanTp_00321}} */Std_ReturnType CanTp_Transmit(PduIdType txPduId, const PduInfoType *pPduInfo);#define CanTp_STOP_SEC_CODE_FAST#include "CanTp_MemMap.h"#define CanTp_START_SEC_CODE_FAST#include "CanTp_MemMap.h"/** * @brief requests cancellation of an ongoing transmission of a PDU in a lower layer communication module. * @param [in] txPduId identification of the PDU to be cancelled * * @retval E_OK cancellation was executed successfully by the destination module * @retval E_NOT_OK cancellation was rejected by the destination module * * @satisfy{@req{SWS_CanTp_00246}} * @satisfy{@req{SWS_CanTp_00254}} * @satisfy{@req{SWS_CanTp_00256}} * @satisfy{@req{SWS_CanTp_00255}} */Std_ReturnType CanTp_CancelTransmit(PduIdType txPduId);#define CanTp_STOP_SEC_CODE_FAST#include "CanTp_MemMap.h"#define CanTp_START_SEC_CODE_FAST#include "CanTp_MemMap.h"/** * @brief requests cancellation of an ongoing reception of a PDU in a lower layer transport protocol module. * @param [in] rxPduId identification of the PDU to be cancelled * * @retval E_OK cancellation was executed successfully by the destination module * @retval E_NOT_OK cancellation was rejected by the destination module * * @satisfy{@req{SWS_CanTp_00257}} * @satisfy{@req{SWS_CanTp_00260}} * @satisfy{@req{SWS_CanTp_00261}} * @satisfy{@req{SWS_CanTp_00262}} * @satisfy{@req{SWS_CanTp_00263}} */Std_ReturnType CanTp_CancelReceive(PduIdType rxPduId);#define CanTp_STOP_SEC_CODE_FAST#include "CanTp_MemMap.h"#define CanTp_START_SEC_CODE_FAST#include "CanTp_MemMap.h"/** * @brief request to change a specific transport protocol parameter (e.g. block size). * @param [in] pduId identification of the PDU which the parameter change shall affect * @param [in] parameter ID of the parameter that shall be changed * @param [in] value the new value of the parameter * * @retval E_OK the parameter was changed successfully * @retval E_NOT_OK the parameter change was rejected * * @satisfy{@req{SWS_CanTp_00302}} * @satisfy{@req{SWS_CanTp_00303}} * @satisfy{@req{SWS_CanTp_00304}} * @satisfy{@req{SWS_CanTp_00338}} * @satisfy{@req{SWS_CanTp_00305}} */Std_ReturnType CanTp_ChangeParameter(PduIdType pduId, TPParameterType parameter, uint16 value);#define CanTp_STOP_SEC_CODE_FAST#include "CanTp_MemMap.h"#define CanTp_START_SEC_CODE_FAST#include "CanTp_MemMap.h"/** * @brief this service is used to read the current value of reception parameters BS and STmin for a specified N-SDU. * @param [in] pduId identifier of the received N-SDU on which the reception parameter are read * @param [in] parameter specify the parameter to which the value has to be read (BS or STmin) * @param [out] pValue pointer where the parameter value will be provided * * @retval E_OK request is accepted * @retval E_NOT_OK request is not accepted * * @satisfy{@req{SWS_CanTp_00323}} * @satisfy{@req{SWS_CanTp_00324}} */Std_ReturnType CanTp_ReadParameter(PduIdType pduId, TPParameterType parameter, uint16 *pValue);#define CanTp_STOP_SEC_CODE_FAST#include "CanTp_MemMap.h"/** @} *//*-----------------------------------------------------------------------------------------------*//* global scheduled function declarations.                                                       *//*-----------------------------------------------------------------------------------------------*//** * @addtogroup CANTP_H_GSFDECL * @{ */#define CanTp_START_SEC_CODE_FAST#include "CanTp_MemMap.h"/** * @brief the main function for scheduling the CAN TP. * @satisfy{@req{SWS_CanTp_00213}} * @satisfy{@req{SWS_CanTp_00164}} * @satisfy{@req{SWS_CanTp_00300}} */void CanTp_MainFunction(void);#define CanTp_STOP_SEC_CODE_FAST#include "CanTp_MemMap.h"/** @} */#ifdef __cplusplus};#endif /* ifdef __cplusplus */#endif /* define CANTP_H */