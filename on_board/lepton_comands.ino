
////////////////////////////////OEM Module////////////////////////////////

/*void oem_runPowerDown(void) {
    sendCommand(cmdCodeOEM(OEM_CID_POWER_DOWN, COMMAND_TYPE_RUN)); 
}
void oem_runPowerUp(void) {
    sendCommand(cmdCodeOEM(0x0000, 0x0000)); 
}

void oem_runCameraReboot(void) {
    sendCommand(cmdCodeOEM(OEM_CID_CAMERA_REBOOT, COMMAND_TYPE_RUN));
}

void oem_getSystemPartNumber(char *buffer, int maxLength) {
    if (!buffer || maxLength < 64) return;

    uint16_t innerBuffer[16];
    receiveCommand(cmdCodeOEM(OEM_CID_FLIR_SYSTEMS_PART_NUMBER, COMMAND_TYPE_GET), innerBuffer, 16);
    wordsToHexString(innerBuffer, 16, buffer, maxLength);
}

void oem_getCameraSoftwareRevision(char *buffer, int maxLength) {
    if (!buffer || maxLength < 16) return;

    uint16_t innerBuffer[4];
    receiveCommand(cmdCodeOEM(OEM_CID_CAMERA_SOFTWARE_REVISION, COMMAND_TYPE_GET), innerBuffer, 4);
    wordsToHexString(innerBuffer, 4, buffer, maxLength);
}

OEM_STATUS oem_getStatus() {
    uint32_t statusoem;
    receiveCommand(cmdCodeOEM(OEM_CID_STATUS, COMMAND_TYPE_GET), &statusoem);
    return (OEM_STATUS)statusoem;
}*/



////////////////////////////////AGC Module////////////////////////////////

void agc_setAGCEnabled(bool enabled) {
    sendCommand(cmdCode(AGC_CID_ENABLE_STATE, COMMAND_TYPE_SET), (uint32_t)enabled);
}

bool agc_getAGCEnabled() {
    uint32_t enabled;
    receiveCommand(cmdCode(AGC_CID_ENABLE_STATE, COMMAND_TYPE_GET), &enabled);
    return enabled;
}

void agc_setAGCPolicy(AGC_POLICY policy) {
    sendCommand(cmdCode(AGC_CID_POLICY, COMMAND_TYPE_SET), (uint32_t)policy);
}

AGC_POLICY agc_getAGCPolicy() {
    uint32_t policy;
    receiveCommand(cmdCode(AGC_CID_POLICY, COMMAND_TYPE_GET), &policy);
    return (AGC_POLICY)policy;
}

void agc_setHEQScaleFactor(AGC_HEQ_SCALE_FACTOR factor) {
    sendCommand(cmdCode(AGC_CID_HEQ_SCALE_FACTOR, COMMAND_TYPE_SET), (uint32_t)factor);
}

AGC_HEQ_SCALE_FACTOR agc_getHEQScaleFactor() {
    uint32_t factor;
    receiveCommand(cmdCode(AGC_CID_HEQ_SCALE_FACTOR, COMMAND_TYPE_GET), &factor);
    return (AGC_HEQ_SCALE_FACTOR)factor;
}

void agc_setAGCCalcEnabled(bool enabled) {
    sendCommand(cmdCode(AGC_CID_CALC_ENABLE_STATE, COMMAND_TYPE_SET), (uint32_t)enabled);
}

bool agc_getAGCCalcEnabled() {
    uint32_t enabled;
    receiveCommand(cmdCode(AGC_CID_CALC_ENABLE_STATE, COMMAND_TYPE_GET), &enabled);
    return enabled;
}





void agc_setHistogramRegion(AGC_HISTOGRAM_ROI *region) {
    if (!region) return;
    sendCommand(cmdCode(AGC_CID_ROI, COMMAND_TYPE_SET), (uint16_t *)region, sizeof(AGC_HISTOGRAM_ROI) / 2);
}

void agc_getHistogramRegion(AGC_HISTOGRAM_ROI *region) {
    if (!region) return;
    receiveCommand(cmdCode(AGC_CID_ROI, COMMAND_TYPE_GET), (uint16_t *)region, sizeof(AGC_HISTOGRAM_ROI) / 2);
}

void agc_getHistogramStatistics(AGC_HISTOGRAM_STATISTICS *statistics) {
    if (!statistics) return;
    receiveCommand(cmdCode(AGC_CID_STATISTICS, COMMAND_TYPE_GET), (uint16_t *)statistics, sizeof(AGC_HISTOGRAM_STATISTICS) / 2);
}

void agc_setHEQDampeningFactor(uint16_t factor) {
    sendCommand(cmdCode(AGC_CID_HEQ_DAMPENING_FACTOR, COMMAND_TYPE_SET), factor);
}

uint16_t agc_getHEQDampeningFactor() {
    uint16_t factor;
    receiveCommand(cmdCode(AGC_CID_HEQ_DAMPENING_FACTOR, COMMAND_TYPE_GET), &factor);
    return factor;
}





void agc_setHistogramClipPercent(uint16_t percent) {
    sendCommand(cmdCode(AGC_CID_HISTOGRAM_CLIP_PERCENT, COMMAND_TYPE_SET), percent);
}

uint16_t agc_getHistogramClipPercent() {
    uint16_t percent;
    receiveCommand(cmdCode(AGC_CID_HISTOGRAM_CLIP_PERCENT, COMMAND_TYPE_GET), &percent);
    return percent;
}

void agc_setHistogramTailSize(uint16_t size) {
    sendCommand(cmdCode(AGC_CID_HISTOGRAM_TAIL_SIZE, COMMAND_TYPE_SET), size);
}

uint16_t agc_getHistogramTailSize() {
    uint16_t size;
    receiveCommand(cmdCode(AGC_CID_HISTOGRAM_TAIL_SIZE, COMMAND_TYPE_GET), &size);
    return size;
}

void agc_setLinearMaxGain(uint16_t gain) {
    sendCommand(cmdCode(AGC_CID_LINEAR_MAX_GAIN, COMMAND_TYPE_SET), gain);
}

uint16_t agc_getLinearMaxGain() {
    uint16_t gain;
    receiveCommand(cmdCode(AGC_CID_LINEAR_MAX_GAIN, COMMAND_TYPE_GET), &gain);
    return gain;
}

void agc_setLinearMidpoint(uint16_t midpoint) {
    sendCommand(cmdCode(AGC_CID_LINEAR_MIDPOINT, COMMAND_TYPE_SET), midpoint);
}

uint16_t agc_getLinearMidpoint() {
    uint16_t midpoint;
    receiveCommand(cmdCode(AGC_CID_LINEAR_MIDPOINT, COMMAND_TYPE_GET), &midpoint);
    return midpoint;
}

void agc_setLinearDampeningFactor(uint16_t factor) {
    sendCommand(cmdCode(AGC_CID_LINEAR_DAMPENING_FACTOR, COMMAND_TYPE_SET), factor);
}

uint16_t agc_getLinearDampeningFactor() {
    uint16_t factor;
    receiveCommand(cmdCode(AGC_CID_LINEAR_DAMPENING_FACTOR, COMMAND_TYPE_GET), &factor);
    return factor;
}



void agc_setHEQMaxGain(uint16_t gain) {
    sendCommand(cmdCode(AGC_CID_HEQ_MAX_GAIN, COMMAND_TYPE_SET), gain);
}

uint16_t agc_getHEQMaxGain() {
    uint16_t gain;
    receiveCommand(cmdCode(AGC_CID_HEQ_MAX_GAIN, COMMAND_TYPE_GET), &gain);
    return gain;
}

void agc_setHEQClipLimitHigh(uint16_t limit) {
    sendCommand(cmdCode(AGC_CID_HEQ_CLIP_LIMIT_HIGH, COMMAND_TYPE_SET), limit);
}

uint16_t agc_getHEQClipLimitHigh() {
    uint16_t limit;
    receiveCommand(cmdCode(AGC_CID_HEQ_CLIP_LIMIT_HIGH, COMMAND_TYPE_GET), &limit);
    return limit;
}

void agc_setHEQClipLimitLow(uint16_t limit) {
    sendCommand(cmdCode(AGC_CID_HEQ_CLIP_LIMIT_LOW, COMMAND_TYPE_SET), limit);
}

uint16_t agc_getHEQClipLimitLow() {
    uint16_t limit;
    receiveCommand(cmdCode(AGC_CID_HEQ_CLIP_LIMIT_LOW, COMMAND_TYPE_GET), &limit);
    return limit;
}

void agc_setHEQBinExtension(uint16_t extension) {
    sendCommand(cmdCode(AGC_CID_HEQ_BIN_EXTENSION, COMMAND_TYPE_SET), extension);
}

uint16_t agc_getHEQBinExtension() {
    uint16_t extension;
    receiveCommand(cmdCode(AGC_CID_HEQ_BIN_EXTENSION, COMMAND_TYPE_GET), &extension);
    return extension;
}

void agc_setHEQMidpoint(uint16_t midpoint) {
    sendCommand(cmdCode(AGC_CID_HEQ_MIDPOINT, COMMAND_TYPE_SET), midpoint);
}

uint16_t agc_getHEQMidpoint() {
    uint16_t midpoint;
    receiveCommand(cmdCode(AGC_CID_HEQ_MIDPOINT, COMMAND_TYPE_GET), &midpoint);
    return midpoint;
}

void agc_setHEQEmptyCounts(uint16_t counts) {
    sendCommand(cmdCode(AGC_CID_HEQ_EMPTY_COUNTS, COMMAND_TYPE_SET), counts);
}

uint16_t agc_getHEQEmptyCounts() {
    uint16_t counts;
    receiveCommand(cmdCode(AGC_CID_HEQ_EMPTY_COUNTS, COMMAND_TYPE_GET), &counts);
    return counts;
}

void agc_setHEQNormalizationFactor(uint16_t factor) {
    sendCommand(cmdCode(AGC_CID_HEQ_NORMALIZATION_FACTOR, COMMAND_TYPE_SET), factor);
}

uint16_t agc_getHEQNormalizationFactor() {
    uint16_t factor;
    receiveCommand(cmdCode(AGC_CID_HEQ_NORMALIZATION_FACTOR, COMMAND_TYPE_GET), &factor);
    return factor;
}





















////////////////////////////////SYS Module////////////////////////////////

void sys_runPingCamera() {
    sendCommand(cmdCode(SYS_CID_PING, COMMAND_TYPE_RUN));
}

bool cameraBooted(){
    uint16_t status; 
    readRegister(STATUS_REG, &status);
    bool cameraBooted = (status & STATUS_BOOT_MODE_BIT_MASK) && (status & STATUS_BOOT_STATUS_BIT_MASK);
    return cameraBooted;
}

void sys_getCameraStatus(SYS_CAM_STATUS *status) {
    receiveCommand(cmdCode(SYS_CID_CAM_STATUS, COMMAND_TYPE_GET), (uint16_t *)status, sizeof(SYS_CAM_STATUS) / 2);
}

SYS_CAM_STATUS_STATES sys_getCameraStatus() {
    SYS_CAM_STATUS camStatus;
    sys_getCameraStatus(&camStatus);
    return (SYS_CAM_STATUS_STATES)camStatus.camStatus;
}

void sys_getFlirSerialNumber(char *buffer, int maxLength) {
    if (!buffer || maxLength < 16) return;

    uint16_t innerBuffer[4];
    receiveCommand(cmdCode(SYS_CID_FLIR_SERIAL_NUMBER, COMMAND_TYPE_GET), innerBuffer, 4);
    wordsToHexString(innerBuffer, 4, buffer, maxLength);
}

void sys_getCustomerSerialNumber(char *buffer, int maxLength) {
    if (!buffer || maxLength < 64) return;

    uint16_t innerBuffer[16];
    receiveCommand(cmdCode(SYS_CID_CUST_SERIAL_NUMBER, COMMAND_TYPE_GET), innerBuffer, 16);
    wordsToHexString(innerBuffer, 16, buffer, maxLength);
}

uint32_t sys_getCameraUptime() {
    uint32_t uptime;
    receiveCommand(cmdCode(SYS_CID_CAM_UPTIME, COMMAND_TYPE_GET), &uptime);
    return uptime;
}

float sys_getAuxTemperature() {
    uint16_t kelvin100;
    receiveCommand(cmdCode(SYS_CID_AUX_TEMPERATURE_KELVIN, COMMAND_TYPE_GET), &kelvin100);
    return kelvin100ToCelsius(kelvin100);
}

float sys_getFPATemperature() {
    uint16_t kelvin100;
    receiveCommand(cmdCode(SYS_CID_FPA_TEMPERATURE_KELVIN, COMMAND_TYPE_GET), &kelvin100);
    return kelvin100ToCelsius(kelvin100);
}

uint16_t sys_getThermalShutdownCount() { //The default value of 270 is just over 10 seconds at 26 Hz video.
    uint16_t count;
    receiveCommand(cmdCode(SYS_CID_THERMAL_SHUTDOWN_COUNT, COMMAND_TYPE_GET), &count);
    return count;
}

void sys_setTelemetryEnabled(bool enabled) {
    sendCommand(cmdCode(SYS_CID_TELEMETRY_ENABLE_STATE, COMMAND_TYPE_SET), (uint32_t)enabled);

    if (!errorwire && !errorlepton) {
        if (enabled && !_telemetryData) {
            _telemetryData = (byte *)malloc(VOSPI_FRAME_SIZE);

            if (_telemetryData)
                _telemetryData[0] = _telemetryData[1] = 0xFF; // initialize as discard packet
        }
        else if (!enabled && _telemetryData) {
            free(_telemetryData);
            _telemetryData = NULL;
        }
    }
}

bool sys_getTelemetryEnabled() {
    uint32_t enabled;
    receiveCommand(cmdCode(SYS_CID_TELEMETRY_ENABLE_STATE, COMMAND_TYPE_GET), &enabled);

    if (!errorwire && !errorlepton) {
        if (enabled && !_telemetryData) {
            _telemetryData = (byte *)malloc(VOSPI_FRAME_SIZE);

            if (_telemetryData)
                _telemetryData[0] = _telemetryData[1] = 0xFF; // initialize as discard packet
        }
        else if (!enabled && _telemetryData) {
            free(_telemetryData);
            _telemetryData = NULL;
        }
    }

    return enabled;
}

bool getShouldRunFFCNormalization() {
    if (!_telemetryData) return false;
    uint16_t *telemetryData = (uint16_t *)&_telemetryData[4];

    uint_fast8_t ffcState = (telemetryData[4] & 0x0018) >> 3;
    if (lowByte(telemetryData[0]) >= 9 && ffcState >= 1)
        ffcState -= 1;

    return (telemetryData[4] & 0x0004) && ffcState != (uint_fast8_t)TelemetryData_FFCState_InProgress;
}

void sys_setTelemetryLocation(SYS_TELEMETRY_LOCATION location) {
    sendCommand(cmdCode(SYS_CID_TELEMETRY_LOCATION, COMMAND_TYPE_SET), (uint32_t)location);
}

SYS_TELEMETRY_LOCATION sys_getTelemetryLocation() {
    uint32_t location;
    receiveCommand(cmdCode(SYS_CID_TELEMETRY_LOCATION, COMMAND_TYPE_GET), &location);
    return (SYS_TELEMETRY_LOCATION)location;
}

void sys_runFFCNormalization() {
    sendCommand(cmdCode(SYS_CID_RUN_FFC, COMMAND_TYPE_RUN));
}

void sys_runFrameAveraging() {
    sendCommand(cmdCode(SYS_CID_EXECTUE_FRAME_AVERAGE, COMMAND_TYPE_RUN));
}

void sys_setNumFramesToAverage(SYS_FRAME_AVERAGE average) {
    sendCommand(cmdCode(SYS_CID_NUM_FRAMES_TO_AVERAGE, COMMAND_TYPE_SET), (uint32_t)average);
}

SYS_FRAME_AVERAGE sys_getNumFramesToAverage() {
    uint32_t average;
    receiveCommand(cmdCode(SYS_CID_NUM_FRAMES_TO_AVERAGE, COMMAND_TYPE_GET), &average);
    return (SYS_FRAME_AVERAGE)average;
}

void sys_getSceneStatistics(SYS_SCENE_STATISTICS *statistics) {
    if (!statistics) return;
    receiveCommand(cmdCode(SYS_CID_SCENE_STATISTICS, COMMAND_TYPE_GET), (uint16_t *)statistics, sizeof(SYS_SCENE_STATISTICS) / 2);
}

void sys_setSceneRegion(SYS_SCENE_ROI *region) {
    if (!region) return;
    sendCommand(cmdCode(SYS_CID_SCENE_ROI, COMMAND_TYPE_SET), (uint16_t *)region, sizeof(SYS_SCENE_ROI) / 2);
}

void sys_getSceneRegion(SYS_SCENE_ROI *region) {
    if (!region) return;
    receiveCommand(cmdCode(SYS_CID_SCENE_ROI, COMMAND_TYPE_GET), (uint16_t *)region, sizeof(SYS_SCENE_ROI) / 2);
}

void sys_setShutterPosition(SYS_SHUTTER_POSITION position) {
    sendCommand(cmdCode(SYS_CID_SHUTTER_POSITION, COMMAND_TYPE_SET), (uint32_t)position);
}

SYS_SHUTTER_POSITION sys_getShutterPosition() {
    uint32_t position;
    receiveCommand(cmdCode(SYS_CID_SHUTTER_POSITION, COMMAND_TYPE_GET), &position);
    return (SYS_SHUTTER_POSITION)position;
}

void sys_setFFCShutterMode(SYS_FFC_SHUTTER_MODE *mode) {
    if (!mode) return;
    sendCommand(cmdCode(SYS_CID_FFC_SHUTTER_MODE, COMMAND_TYPE_SET), (uint16_t *)mode, sizeof(SYS_FFC_SHUTTER_MODE) / 2);
}

void sys_getFFCShutterMode(SYS_FFC_SHUTTER_MODE *mode) {
    if (!mode) return;
    receiveCommand(cmdCode(SYS_CID_FFC_SHUTTER_MODE, COMMAND_TYPE_GET), (uint16_t *)mode, sizeof(SYS_FFC_SHUTTER_MODE) / 2);
}

SYS_FFC_STATUS sys_getFFCNormalizationStatus() {
    uint32_t ffcstatus;
    receiveCommand(cmdCode(SYS_CID_FFC_STATUS, COMMAND_TYPE_GET), &ffcstatus);
    return (SYS_FFC_STATUS)ffcstatus;
}

SYS_GAIN_MODE sys_getGainMode() {
    uint32_t gainmode;
    receiveCommand(cmdCode(SYS_CID_GAIN_MODE, COMMAND_TYPE_GET), &gainmode);
    return (SYS_GAIN_MODE)gainmode;
}

void sys_setGainMode(SYS_GAIN_MODE gainmode) {
    sendCommand(cmdCode(SYS_CID_GAIN_MODE, COMMAND_TYPE_SET), (uint32_t)gainmode);
}

SYS_FFC_STATES sys_getFFCStates() {
    uint32_t ffcstates;
    receiveCommand(cmdCode(SYS_CID_FFC_STATES, COMMAND_TYPE_GET), &ffcstates);
    return (SYS_FFC_STATES)ffcstates;
}

void sys_setFFCStates(SYS_FFC_STATES ffcstates) {
    sendCommand(cmdCode(SYS_CID_FFC_STATES, COMMAND_TYPE_SET), (uint32_t)ffcstates);
}



////////////////////////////////VID Module////////////////////////////////

void vid_setPolarity(VID_POLARITY polarity) {
    sendCommand(cmdCode(VID_CID_POLARITY_SELECT, COMMAND_TYPE_SET), (uint32_t)polarity);
}

VID_POLARITY vid_getPolarity() {
    uint32_t polarity;
    receiveCommand(cmdCode(VID_CID_POLARITY_SELECT, COMMAND_TYPE_GET), &polarity);
    return (VID_POLARITY)polarity;
}

void vid_setPseudoColorLUT(VID_PCOLOR_LUT table) {
    sendCommand(cmdCode(VID_CID_LUT_SELECT, COMMAND_TYPE_SET), (uint32_t)table);
}

VID_PCOLOR_LUT vid_getPseudoColorLUT() {
    uint32_t table;
    receiveCommand(cmdCode(VID_CID_LUT_SELECT, COMMAND_TYPE_GET), &table);
    return (VID_PCOLOR_LUT)table;
}

void vid_setFocusCalcEnabled(bool enabled) {
    sendCommand(cmdCode(VID_CID_FOCUS_CALC_ENABLE, COMMAND_TYPE_SET), (uint32_t)enabled);
}

bool vid_getFocusCalcEnabled() {
    uint32_t enabled;
    receiveCommand(cmdCode(VID_CID_FOCUS_CALC_ENABLE, COMMAND_TYPE_GET), &enabled);
    return enabled;
}

void vid_setFreezeEnabled(bool enabled) {
    sendCommand(cmdCode(VID_CID_FREEZE_ENABLE, COMMAND_TYPE_SET), (uint32_t)enabled);
}

bool vid_getFreezeEnabled() {
    uint32_t enabled;
    receiveCommand(cmdCode(VID_CID_FREEZE_ENABLE, COMMAND_TYPE_GET), &enabled);
    return enabled;
}

void vid_setUserColorLUT(VID_LUT_BUFFER *table) {
    if (!table) return;
    sendCommand(cmdCode(VID_CID_LUT_TRANSFER, COMMAND_TYPE_SET), (uint16_t *)table, sizeof(VID_LUT_BUFFER) / 2);
}

void vid_getUserColorLUT(VID_LUT_BUFFER *table) {
    if (!table) return;
    receiveCommand(cmdCode(VID_CID_LUT_TRANSFER, COMMAND_TYPE_GET), (uint16_t *)table, sizeof(VID_LUT_BUFFER) / 2);
}

void vid_setFocusRegion(VID_FOCUS_ROI *region) {
    if (!region) return;
    sendCommand(cmdCode(VID_CID_FOCUS_ROI, COMMAND_TYPE_SET), (uint16_t *)region, sizeof(VID_FOCUS_ROI) / 2);
}

void vid_getFocusRegion(VID_FOCUS_ROI *region) {
    if (!region) return;
    receiveCommand(cmdCode(VID_CID_FOCUS_ROI, COMMAND_TYPE_GET), (uint16_t *)region, sizeof(VID_FOCUS_ROI) / 2);
}

void vid_setFocusThreshold(uint32_t threshold) {
    sendCommand(cmdCode(VID_CID_FOCUS_THRESHOLD, COMMAND_TYPE_SET), threshold);
}

uint32_t vid_getFocusThreshold() {
    uint32_t threshold;
    receiveCommand(cmdCode(VID_CID_FOCUS_THRESHOLD, COMMAND_TYPE_GET), &threshold);
    return threshold;
}

uint32_t vid_getFocusMetric() {
    uint32_t metric;
    receiveCommand(cmdCode(VID_CID_FOCUS_METRIC, COMMAND_TYPE_GET), &metric);
    return metric;
}





void vid_setSceneBasedNUCEnabled(bool enabled) {
    sendCommand(cmdCode(VID_CID_SBNUC_ENABLE, COMMAND_TYPE_SET), (uint32_t)enabled);
}

bool vid_getSceneBasedNUCEnabled() {
    uint32_t enabled;
    receiveCommand(cmdCode(VID_CID_SBNUC_ENABLE, COMMAND_TYPE_GET), &enabled);
    return enabled;
}

void vid_setGamma(uint32_t gamma) {
    sendCommand(cmdCode(VID_CID_GAMMA_SELECT, COMMAND_TYPE_SET), gamma);
}

uint32_t vid_getGamma() {
    uint32_t gamma;
    receiveCommand(cmdCode(VID_CID_GAMMA_SELECT, COMMAND_TYPE_GET), &gamma);
    return gamma;
}

/*void vid_setVideoFormat(VID_VIDEO_OUTPUT_FORMAT videoformat) {
    sendCommand(cmdCode(VID_CID_VIDEO_FORMAT, COMMAND_TYPE_SET), videoformat);
}

VID_VIDEO_OUTPUT_FORMAT vid_getVideoFormat() {
    uint32_t videoformat;
    receiveCommand(cmdCode(VID_CID_VIDEO_FORMAT, COMMAND_TYPE_GET), &videoformat);
    return (VID_VIDEO_OUTPUT_FORMAT) videoformat;
}*/

void vid_setpcolorlut(VID_PCOLOR_LUT pcolorlut) {
    sendCommand(cmdCode(VID_CID_PCOLOR_LUT, COMMAND_TYPE_SET), (uint32_t)pcolorlut);
}

VID_PCOLOR_LUT vid_getpcolorlut() {
    uint32_t pcolorlut;
    receiveCommand(cmdCode(VID_CID_PCOLOR_LUT, COMMAND_TYPE_GET), &pcolorlut);
    return (VID_PCOLOR_LUT) pcolorlut;
}

