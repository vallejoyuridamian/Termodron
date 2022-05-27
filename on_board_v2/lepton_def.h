byte errorwire = 0;   //variable que almacena el tipo de error Wire
byte errorlepton = 0;   //variable que almacena el tipo de error de la Lepton
byte *_telemetryData;   


#define GEN_CMD_TIMEOUT         5000        // Timeout for commands to be processed
#define VOSPI_FRAME_SIZE           164 // 2B ID + 2B CRC + 160B for 80x1 14bpp/8bppAGC thermal image data or telemetry data

byte lepton_frame_packet[VOSPI_FRAME_SIZE];

#define DEVICE_ADDRESS                  (byte)0x2A

#define COMMAND_MODULE_ID_BIT_MASK      (uint16_t)0x0F00
#define COMMAND_ID_BIT_MASK             (uint16_t)0x00FC
#define COMMAND_TYPE_BIT_MASK           (uint16_t)0x0003
#define COMMAND_PROTECTION_BIT_MASK     (uint16_t)0xF000

#define OEM_PROTECTION_BIT              (uint16_t)0x4000

#define COMMAND_TYPE_GET                (uint16_t)0x0000
#define COMMAND_TYPE_SET                (uint16_t)0x0001
#define COMMAND_TYPE_RUN                (uint16_t)0x0002

#define STATUS_BUSY_BIT_MASK            (uint16_t)0x0001
#define STATUS_BOOT_MODE_BIT_MASK       (uint16_t)0x0002
#define STATUS_BOOT_STATUS_BIT_MASK     (uint16_t)0x0004
#define STATUS_ERROR_CODE_BIT_MASK      (uint16_t)0xFF00
#define STATUS_ERROR_CODE_BIT_SHIFT     8

#define REG_BASE_ADDR                   (uint16_t)0x0000
#define POWER_REG                       (uint16_t)(REG_BASE_ADDR + 0x0000)
#define STATUS_REG                      (uint16_t)(REG_BASE_ADDR + 0x0002)
#define COMMAND_REG                     (uint16_t)(REG_BASE_ADDR + 0x0004)
#define DATA_LENGTH_REG                 (uint16_t)(REG_BASE_ADDR + 0x0006)
#define DATA_0_REG                      (uint16_t)(REG_BASE_ADDR + 0x0008)
#define DATA_1_REG                      (uint16_t)(REG_BASE_ADDR + 0x000A)
#define DATA_2_REG                      (uint16_t)(REG_BASE_ADDR + 0x000C)
#define DATA_3_REG                      (uint16_t)(REG_BASE_ADDR + 0x000E)
#define DATA_4_REG                      (uint16_t)(REG_BASE_ADDR + 0x0010)
#define DATA_5_REG                      (uint16_t)(REG_BASE_ADDR + 0x0012)
#define DATA_6_REG                      (uint16_t)(REG_BASE_ADDR + 0x0014)
#define DATA_7_REG                      (uint16_t)(REG_BASE_ADDR + 0x0016)
#define DATA_8_REG                      (uint16_t)(REG_BASE_ADDR + 0x0018)
#define DATA_9_REG                      (uint16_t)(REG_BASE_ADDR + 0x001A)
#define DATA_10_REG                     (uint16_t)(REG_BASE_ADDR + 0x001C)
#define DATA_11_REG                     (uint16_t)(REG_BASE_ADDR + 0x001E)
#define DATA_12_REG                     (uint16_t)(REG_BASE_ADDR + 0x0020)
#define DATA_13_REG                     (uint16_t)(REG_BASE_ADDR + 0x0022)
#define DATA_14_REG                     (uint16_t)(REG_BASE_ADDR + 0x0024)
#define DATA_15_REG                     (uint16_t)(REG_BASE_ADDR + 0x0026)
#define DATA_CRC_REG                    (uint16_t)(REG_BASE_ADDR + 0x0028)

#define DATA_BUFFER                     (uint16_t)0xF800
#define DATA_BUFFER_LENGTH              (uint16_t)0x0800

#define AGC_CID_MODULE_BASE                 (uint16_t)0x0100
#define AGC_CID_ENABLE_STATE                (uint16_t)(AGC_CID_MODULE_BASE + 0x0000)
#define AGC_CID_POLICY                      (uint16_t)(AGC_CID_MODULE_BASE + 0x0004)
#define AGC_CID_ROI                         (uint16_t)(AGC_CID_MODULE_BASE + 0x0008)
#define AGC_CID_STATISTICS                  (uint16_t)(AGC_CID_MODULE_BASE + 0x000C)
#define AGC_CID_HISTOGRAM_CLIP_PERCENT      (uint16_t)(AGC_CID_MODULE_BASE + 0x0010)
#define AGC_CID_HISTOGRAM_TAIL_SIZE         (uint16_t)(AGC_CID_MODULE_BASE + 0x0014)
#define AGC_CID_LINEAR_MAX_GAIN             (uint16_t)(AGC_CID_MODULE_BASE + 0x0018)
#define AGC_CID_LINEAR_MIDPOINT             (uint16_t)(AGC_CID_MODULE_BASE + 0x001C)
#define AGC_CID_LINEAR_DAMPENING_FACTOR     (uint16_t)(AGC_CID_MODULE_BASE + 0x0020)
#define AGC_CID_HEQ_DAMPENING_FACTOR        (uint16_t)(AGC_CID_MODULE_BASE + 0x0024)
#define AGC_CID_HEQ_MAX_GAIN                (uint16_t)(AGC_CID_MODULE_BASE + 0x0028)
#define AGC_CID_HEQ_CLIP_LIMIT_HIGH         (uint16_t)(AGC_CID_MODULE_BASE + 0x002C)
#define AGC_CID_HEQ_CLIP_LIMIT_LOW          (uint16_t)(AGC_CID_MODULE_BASE + 0x0030)
#define AGC_CID_HEQ_BIN_EXTENSION           (uint16_t)(AGC_CID_MODULE_BASE + 0x0034)
#define AGC_CID_HEQ_MIDPOINT                (uint16_t)(AGC_CID_MODULE_BASE + 0x0038)
#define AGC_CID_HEQ_EMPTY_COUNTS            (uint16_t)(AGC_CID_MODULE_BASE + 0x003C)
#define AGC_CID_HEQ_NORMALIZATION_FACTOR    (uint16_t)(AGC_CID_MODULE_BASE + 0x0040)
#define AGC_CID_HEQ_SCALE_FACTOR            (uint16_t)(AGC_CID_MODULE_BASE + 0x0044)
#define AGC_CID_CALC_ENABLE_STATE           (uint16_t)(AGC_CID_MODULE_BASE + 0x0048)

typedef enum {
    AGC_LINEAR = 0,
    AGC_HEQ,
} AGC_POLICY;

typedef struct {
    uint16_t startCol;
    uint16_t startRow;
    uint16_t endCol;
    uint16_t endRow;
} AGC_HISTOGRAM_ROI;

typedef struct {
    uint16_t minIntensity;
    uint16_t maxIntensity;
    uint16_t meanIntensity;
    uint16_t numPixels; // def: 4800
} AGC_HISTOGRAM_STATISTICS;

typedef enum {
    AGC_SCALE_TO_8_BITS = 0,
    AGC_SCALE_TO_14_BITS
} AGC_HEQ_SCALE_FACTOR;


#define SYS_CID_MODULE_BASE                 (uint16_t)0x0200
#define SYS_CID_PING                        (uint16_t)(SYS_CID_MODULE_BASE + 0x0000)
#define SYS_CID_CAM_STATUS                  (uint16_t)(SYS_CID_MODULE_BASE + 0x0004)
#define SYS_CID_FLIR_SERIAL_NUMBER          (uint16_t)(SYS_CID_MODULE_BASE + 0x0008)
#define SYS_CID_CAM_UPTIME                  (uint16_t)(SYS_CID_MODULE_BASE + 0x000C)
#define SYS_CID_AUX_TEMPERATURE_KELVIN      (uint16_t)(SYS_CID_MODULE_BASE + 0x0010)
#define SYS_CID_FPA_TEMPERATURE_KELVIN      (uint16_t)(SYS_CID_MODULE_BASE + 0x0014)
#define SYS_CID_TELEMETRY_ENABLE_STATE      (uint16_t)(SYS_CID_MODULE_BASE + 0x0018)
#define SYS_CID_TELEMETRY_LOCATION          (uint16_t)(SYS_CID_MODULE_BASE + 0x001C)
#define SYS_CID_EXECTUE_FRAME_AVERAGE       (uint16_t)(SYS_CID_MODULE_BASE + 0x0020)
#define SYS_CID_NUM_FRAMES_TO_AVERAGE       (uint16_t)(SYS_CID_MODULE_BASE + 0x0024)
#define SYS_CID_CUST_SERIAL_NUMBER          (uint16_t)(SYS_CID_MODULE_BASE + 0x0028)
#define SYS_CID_SCENE_STATISTICS            (uint16_t)(SYS_CID_MODULE_BASE + 0x002C)
#define SYS_CID_SCENE_ROI                   (uint16_t)(SYS_CID_MODULE_BASE + 0x0030)
#define SYS_CID_THERMAL_SHUTDOWN_COUNT      (uint16_t)(SYS_CID_MODULE_BASE + 0x0034)
#define SYS_CID_SHUTTER_POSITION            (uint16_t)(SYS_CID_MODULE_BASE + 0x0038)
#define SYS_CID_FFC_SHUTTER_MODE            (uint16_t)(SYS_CID_MODULE_BASE + 0x003C)
#define SYS_CID_RUN_FFC                     (uint16_t)(SYS_CID_MODULE_BASE + 0x0042)
#define SYS_CID_FFC_STATUS                  (uint16_t)(SYS_CID_MODULE_BASE + 0x0044)
#define SYS_CID_GAIN_MODE                   (uint16_t)(SYS_CID_MODULE_BASE + 0x0048)
#define SYS_CID_FFC_STATES                  (uint16_t)(SYS_CID_MODULE_BASE + 0x004C)

typedef enum {
    TelemetryData_FFCState_NeverCommanded,
    TelemetryData_FFCState_InProgress,
    TelemetryData_FFCState_Complete
} TelemetryData_FFCState;

typedef enum {
    SYSTEM_READY = 0,
    SYSTEM_INITIALIZING,
    SYSTEM_IN_LOW_POWER_MODE,
    SYSTEM_GOING_INTO_STANDBY,
    SYSTEM_FLAT_FIELD_IN_PROCESS
} SYS_CAM_STATUS_STATES;

typedef struct {
    uint32_t camStatus; // SYS_CAM_STATUS_STATES
    uint16_t commandCount;
    uint16_t reserved;
} SYS_CAM_STATUS;

typedef enum {
    TELEMETRY_LOCATION_HEADER = 0,
    TELEMETRY_LOCATION_FOOTER
} SYS_TELEMETRY_LOCATION;

typedef enum {
    SYS_FA_DIV_1 = 0,
    SYS_FA_DIV_2,
    SYS_FA_DIV_4,
    SYS_FA_DIV_8,
    SYS_FA_DIV_16,
    SYS_FA_DIV_32,
    SYS_FA_DIV_64,
    SYS_FA_DIV_128
} SYS_FRAME_AVERAGE;

typedef struct {
    uint16_t meanIntensity;
    uint16_t maxIntensity;
    uint16_t minIntensity;
    uint16_t numPixels;
} SYS_SCENE_STATISTICS;

typedef struct {
    uint16_t startCol;
    uint16_t startRow;
    uint16_t endCol;
    uint16_t endRow;
} SYS_SCENE_ROI;

typedef enum {
    SYS_SHUTTER_POSITION_UNKNOWN = -1,
    SYS_SHUTTER_POSITION_IDLE = 0,
    SYS_SHUTTER_POSITION_OPEN,
    SYS_SHUTTER_POSITION_CLOSED,
    SYS_SHUTTER_POSITION_BRAKE_ON
} SYS_SHUTTER_POSITION;

typedef enum {
    SYS_FFC_SHUTTER_MODE_MANUAL = 0,
    SYS_FFC_SHUTTER_MODE_AUTO,
    SYS_FFC_SHUTTER_MODE_EXTERNAL
} SYS_FFC_SHUTTER_MODE_STATE;

typedef enum {
    SYS_SHUTTER_LOCKOUT_INACTIVE = 0,
    SYS_SHUTTER_LOCKOUT_HIGH,
    SYS_SHUTTER_LOCKOUT_LOW
} SYS_SHUTTER_TEMP_LOCKOUT_STATE;

typedef struct {
    uint32_t shutterMode;               // SYS_FFC_SHUTTER_MODE_STATE def:SYS_FFC_SHUTTER_MODE_EXTERNAL
    uint32_t tempLockoutState;          // SYS_SHUTTER_TEMP_LOCKOUT_STATE def:SYS_SHUTTER_LOCKOUT_INACTIVE
    uint32_t videoFreezeDuringFFC;      // bool def:enabled
    uint32_t ffcDesired;                // bool def:disabled
    uint32_t elapsedTimeSinceLastFFC;   // (ms)
    uint32_t desiredFFCPeriod;          // def:300000 (ms)
    uint32_t explicitCmdToOpen;         // bool def:disabled
    uint16_t desiredFFCTempDelta;       // def:300 (kelvins*100)
    uint16_t imminentDelay;             // def:52 (frame counts)
} SYS_FFC_SHUTTER_MODE;

typedef enum {
    SYS_FFC_STATUS_WRITE_ERROR = -2,
    SYS_FFC_STATUS_ERROR = -1,
    SYS_FFC_STATUS_READY = 0,
    SYS_FFC_STATUS_BUSY,
    SYS_FRAME_AVERAGE_COLLECTING_FRAMES,
} SYS_FFC_STATUS;

typedef enum {
    SYS_GAIN_MODE_HIGH = 0,
    SYS_GAIN_MODE_LOW,
    SYS_GAIN_MODE_AUTO,
    SYS_END_GAIN_MODE,
} SYS_GAIN_MODE;

typedef enum {
    SYS_FFC_NEVER_COMMANDED = 0,
    SYS_FFC_IMMINENT,
    SYS_FFC_IN_PROCESS,
    SYS_FFC_DONE,
    SYS_END_FFC_STATES
} SYS_FFC_STATES;


#define VID_CID_MODULE_BASE                 (uint16_t)0x0300
#define VID_CID_POLARITY_SELECT             (uint16_t)(VID_CID_MODULE_BASE + 0x0000)
#define VID_CID_LUT_SELECT                  (uint16_t)(VID_CID_MODULE_BASE + 0x0004)
#define VID_CID_LUT_TRANSFER                (uint16_t)(VID_CID_MODULE_BASE + 0x0008)
#define VID_CID_FOCUS_CALC_ENABLE           (uint16_t)(VID_CID_MODULE_BASE + 0x000C)
#define VID_CID_FOCUS_ROI                   (uint16_t)(VID_CID_MODULE_BASE + 0x0010)
#define VID_CID_FOCUS_THRESHOLD             (uint16_t)(VID_CID_MODULE_BASE + 0x0014)
#define VID_CID_FOCUS_METRIC                (uint16_t)(VID_CID_MODULE_BASE + 0x0018)
#define VID_CID_SBNUC_ENABLE                (uint16_t)(VID_CID_MODULE_BASE + 0x001C)
#define VID_CID_GAMMA_SELECT                (uint16_t)(VID_CID_MODULE_BASE + 0x0020)
#define VID_CID_FREEZE_ENABLE               (uint16_t)(VID_CID_MODULE_BASE + 0x0024)
#define VID_CID_VIDEO_FORMAT                (uint16_t)(VID_CID_MODULE_BASE + 0x0030)
#define VID_CID_PCOLOR_LUT                  (uint16_t)(VID_CID_MODULE_BASE + 0x0034)

typedef enum {
    VID_WHITE_HOT = 0,
    VID_BLACK_HOT
} VID_POLARITY;

typedef enum {
    VID_WHEEL6_LUT = 0,
    VID_FUSION_LUT,
    VID_RAINBOW_LUT,
    VID_GLOBOW_LUT,
    VID_SEPIA_LUT,
    VID_COLOR_LUT,
    VID_ICE_FIRE_LUT,
    VID_RAIN_LUT,
    VID_USER_LUT,
} VID_PCOLOR_LUT;

typedef struct {
    uint8_t reserved;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} VID_LUT_PIXEL;

typedef struct {
    VID_LUT_PIXEL bin[256];
} VID_LUT_BUFFER;

typedef struct {
    uint16_t startCol;
    uint16_t startRow;
    uint16_t endCol;
    uint16_t endRow;
} VID_FOCUS_ROI;

/*typedef struct {
    VID_VIDEO_OUTPUT_FORMAT_RAW8 = 0, // To be supported in later release
    VID_VIDEO_OUTPUT_FORMAT_RAW10, // To be supported in later release
    VID_VIDEO_OUTPUT_FORMAT_RAW12, // To be supported in later release
    VID_VIDEO_OUTPUT_FORMAT_RGB888, // To be supported in later release
    VID_VIDEO_OUTPUT_FORMAT_RGB666, // To be supported in later release
    VID_VIDEO_OUTPUT_FORMAT_RGB565, // To be supported in later release
    VID_VIDEO_OUTPUT_FORMAT_YUV422_8BIT, // To be supported in later release
    VID_VIDEO_OUTPUT_FORMAT_RAW14, // SUPPORTED in this release
    VID_VIDEO_OUTPUT_FORMAT_YUV422_10BIT, // To be supported in later release
    VID_VIDEO_OUTPUT_FORMAT_USER_DEFINED, // To be supported in later release
    VID_VIDEO_OUTPUT_FORMAT_RAW8_2, // To be supported in later release
    VID_VIDEO_OUTPUT_FORMAT_RAW8_3, // To be supported in later release
    VID_VIDEO_OUTPUT_FORMAT_RAW8_4, // To be supported in later release
    VID_VIDEO_OUTPUT_FORMAT_RAW8_5, // To be supported in later release
    VID_VIDEO_OUTPUT_FORMAT_RAW8_6, // To be supported in later release
    END_VID_VIDEO_OUTPUT_FORMAT 
}VID_VIDEO_OUTPUT_FORMAT;*/



#define OEM_CID_MODULE_BASE                 (uint16_t)0x0800
#define OEM_CID_POWER_DOWN                  (uint16_t)(OEM_CID_MODULE_BASE + 0x0000)
#define OEM_CID_FLIR_SYSTEMS_PART_NUMBER    (uint16_t)(OEM_CID_MODULE_BASE + 0x001C)
#define OEM_CID_CAMERA_SOFTWARE_REVISION    (uint16_t)(OEM_CID_MODULE_BASE + 0x0020)
#define OEM_CID_VIDEO_OUTPUT_ENABLE         (uint16_t)(OEM_CID_MODULE_BASE + 0x0024)
#define OEM_CID_VIDEO_OUTPUT_FORMAT_SELECT  (uint16_t)(OEM_CID_MODULE_BASE + 0x0028)
#define OEM_CID_VIDEO_OUTPUT_SOURCE_SELECT  (uint16_t)(OEM_CID_MODULE_BASE + 0x002c)
#define OEM_CID_CUSTOMER_PART_NUMBER        (uint16_t)(OEM_CID_MODULE_BASE + 0x0038)
#define OEM_CID_VIDEO_OUTPUT_SOURCE_CONST_VALUE                (uint16_t)(OEM_CID_MODULE_BASE + 0x003c)
#define OEM_CID_CAMERA_REBOOT               (uint16_t)(OEM_CID_MODULE_BASE + 0x0040)
#define OEM_CID_FFC_NORMALIZATION_TARGET    (uint16_t)(OEM_CID_MODULE_BASE + 0x0044)
#define OEM_CID_STATUS                      (uint16_t)(OEM_CID_MODULE_BASE + 0x0048)
#define OEM_CID_FRAME_MEAN_INTENSITY        (uint16_t)(OEM_CID_MODULE_BASE + 0x004C)
#define OEM_CID_GPIO_MODE_SELECT            (uint16_t)(OEM_CID_MODULE_BASE + 0x0054)
#define OEM_CID_GPIO_VSYNC_PHASE_DELAY      (uint16_t)(OEM_CID_MODULE_BASE + 0x0058)
#define OEM_CID_USER_DEFAULTS               (uint16_t)(OEM_CID_MODULE_BASE + 0x005C)
#define OEM_CID_RESTORE_USER_DEFAULTS       (uint16_t)(OEM_CID_MODULE_BASE + 0x0060)
#define OEM_CID_SHUTTER_PROFILE             (uint16_t)(OEM_CID_MODULE_BASE + 0x0064)
#define OEM_CID_THERMAL_SHUTDOWN_ENABLE     (uint16_t)(OEM_CID_MODULE_BASE + 0x0068)
#define OEM_CID_BAD_PIXEL_REPLACEMENT_CONTROL                (uint16_t)(OEM_CID_MODULE_BASE + 0x006C)
#define OEM_CID_TEMPORAL_FILTER_CONTROL     (uint16_t)(OEM_CID_MODULE_BASE + 0x0070)
#define OEM_CID_COLUMN_NOISE_FILTER_CONTROL (uint16_t)(OEM_CID_MODULE_BASE + 0x0074)
#define OEM_CID_PIXEL_NOISE_FILTER_CONTROL  (uint16_t)(OEM_CID_MODULE_BASE + 0x0078)




typedef enum{
 OEM_STATUS_OTP_WRITE_ERROR = -2,
 OEM_STATUS_ERROR = -1,
 OEM_STATUS_READY = 0,
 OEM_STATUS_BUSY,
 OEM_FRAME_AVERAGE_COLLECTING_FRAMES,
 OEM_STATUS_END
} OEM_STATUS;





typedef enum {
    LEP_OK = 0,     /* Camera ok */
    LEP_COMM_OK = LEP_OK, /* Camera comm ok (same as LEP_OK) */

    LEP_ERROR = -1,    /* Camera general error */
    LEP_NOT_READY = -2,    /* Camera not ready error */
    LEP_RANGE_ERROR = -3,    /* Camera range error */
    LEP_CHECKSUM_ERROR = -4,    /* Camera checksum error */
    LEP_BAD_ARG_POINTER_ERROR = -5,    /* Camera Bad argument  error */
    LEP_DATA_SIZE_ERROR = -6,    /* Camera byte count error */
    LEP_UNDEFINED_FUNCTION_ERROR = -7,    /* Camera undefined function error */
    LEP_FUNCTION_NOT_SUPPORTED = -8,    /* Camera function not yet supported error */

    /* OTP access errors */
    LEP_OTP_WRITE_ERROR = -15,   /*!< Camera OTP write error */
    LEP_OTP_READ_ERROR = -16,   /* double bit error detected (uncorrectible) */

    LEP_OTP_NOT_PROGRAMMED_ERROR = -18,   /* Flag read as non-zero */

    /* I2C Errors */
    LEP_ERROR_I2C_BUS_NOT_READY = -20,   /* I2C Bus Error - Bus Not Avaialble */
    LEP_ERROR_I2C_BUFFER_OVERFLOW = -22,   /* I2C Bus Error - Buffer Overflow */
    LEP_ERROR_I2C_ARBITRATION_LOST = -23,   /* I2C Bus Error - Bus Arbitration Lost */
    LEP_ERROR_I2C_BUS_ERROR = -24,   /* I2C Bus Error - General Bus Error */
    LEP_ERROR_I2C_NACK_RECEIVED = -25,   /* I2C Bus Error - NACK Received */
    LEP_ERROR_I2C_FAIL = -26,   /* I2C Bus Error - General Failure */

    /* Processing Errors */
    LEP_DIV_ZERO_ERROR = -80,   /* Attempted div by zero */

    /* Comm Errors */
    LEP_COMM_PORT_NOT_OPEN = -101,  /* Comm port not open */
    LEP_COMM_INVALID_PORT_ERROR = -102,  /* Comm port no such port error */
    LEP_COMM_RANGE_ERROR = -103,  /* Comm port range error */
    LEP_ERROR_CREATING_COMM = -104,  /* Error creating comm */
    LEP_ERROR_STARTING_COMM = -105,  /* Error starting comm */
    LEP_ERROR_CLOSING_COMM = -106,  /* Error closing comm */
    LEP_COMM_CHECKSUM_ERROR = -107,  /* Comm checksum error */
    LEP_COMM_NO_DEV = -108,  /* No comm device */
    LEP_TIMEOUT_ERROR = -109,  /* Comm timeout error */
    LEP_COMM_ERROR_WRITING_COMM = -110,  /* Error writing comm */
    LEP_COMM_ERROR_READING_COMM = -111,  /* Error reading comm */
    LEP_COMM_COUNT_ERROR = -112,  /* Comm byte count error */

    /* Other Errors */
    LEP_OPERATION_CANCELED = -126,  /* Camera operation canceled */
    LEP_UNDEFINED_ERROR_CODE = -127   /* Undefined error */
} LEP_RESULT;

