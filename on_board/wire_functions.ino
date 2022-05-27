

////////////////////////////////WIRE Functions////////////////////////////////

void Wire_beginTransmission(uint8_t addr) {
    errorwire = 0;
    Wire.beginTransmission(addr);
}

int Wire_endTransmission(void) {
    errorwire = Wire.endTransmission();
    return (errorwire);
}

uint8_t Wire_requestFrom(uint8_t addr, uint8_t len) {
    return Wire.requestFrom(addr, len);
}

size_t Wire_write(uint8_t data) {
    return Wire.write(data);
}

size_t Wire_write16(uint16_t data) {
    return Wire.write(highByte(data)) + Wire.write(lowByte(data));
}

uint8_t Wire_read(void) {
    return (uint8_t)(Wire.read() & 0xFF);
}

uint16_t Wire_read16(void) {
    return ((uint16_t)(Wire.read() & 0xFF) << 8) | (uint16_t)(Wire.read() & 0xFF);
}


int writeCmdRegister(uint16_t cmdCode, uint16_t *dataWords, int dataLength) {
    if (dataWords && dataLength) {
        Wire_beginTransmission(DEVICE_ADDRESS);
        Wire_write16(DATA_LENGTH_REG);
        Wire_write16(dataLength);
        if (Wire_endTransmission())
            return errorwire;

        int maxLength = BUFFER_LENGTH / 2;
        int writeLength = min(maxLength, dataLength);
        uint16_t regAddress = dataLength <= 16 ? DATA_0_REG : DATA_BUFFER;

        while (dataLength > 0) {
            Wire_beginTransmission(DEVICE_ADDRESS);
            Wire_write16(regAddress);

            while (writeLength-- > 0)
                Wire_write16(*dataWords++);

            if (Wire_endTransmission())
                return errorwire;

            regAddress += maxLength * 0x02;
            dataLength -= maxLength;
            writeLength = min(maxLength, dataLength);
        }
    }

    Wire_beginTransmission(DEVICE_ADDRESS);
    Wire_write16(COMMAND_REG);
    Wire_write16(cmdCode);
    return Wire_endTransmission();
}

int readDataRegister(uint16_t *readWords, int maxLength) {
    Wire_beginTransmission(DEVICE_ADDRESS);
    Wire_write16(DATA_LENGTH_REG);
    if (Wire_endTransmission())
        return errorwire;

    int bytesRead = Wire_requestFrom(DEVICE_ADDRESS, 2);
    if (bytesRead != 2) {
        while (bytesRead-- > 0)
            Wire_read();
        return (errorwire = 4);
    }

    int readLength = Wire_read16();

    if (readLength == 0)
        return (errorwire = 4);

    bytesRead = Wire_requestFrom(DEVICE_ADDRESS, min(BUFFER_LENGTH, readLength));

    while (bytesRead > 0 && readLength > 0) {
        while (bytesRead > 1 && readLength > 1 && maxLength > 0) {
            *readWords++ = Wire_read16();
            bytesRead -= 2;
            readLength -= 2;
            --maxLength;
        }
        if (readLength > 0)
            bytesRead += Wire_requestFrom(DEVICE_ADDRESS, min(BUFFER_LENGTH, readLength));
    }

    while (bytesRead-- > 0)
        Wire_read();

    while (maxLength-- > 0)
        *readWords++ = 0;

    return (errorwire = readLength ? 4 : 0);
}

int writeRegister(uint16_t regAddress, uint16_t value) {
    Wire_beginTransmission(DEVICE_ADDRESS);
    Wire_write16(regAddress);
    Wire_write16(value);
    return Wire_endTransmission();
}

int readRegister(uint16_t regAddress, uint16_t *value) {
    Wire_beginTransmission(DEVICE_ADDRESS);
    Wire_write16(regAddress);
    if (Wire_endTransmission())
        return errorwire;

    int bytesRead = Wire_requestFrom(DEVICE_ADDRESS, 2);
    if (bytesRead != 2) {
        while (bytesRead-- > 0)
            Wire_read();
        return (errorwire = 4);
    }

    *value = Wire_read16();
    return errorwire;
}










bool waitCommandBegin(int timeout_lep) {
    errorlepton = 0;
    uint16_t status;
    if (readRegister(STATUS_REG, &status))
        return false;

    if (!(status & STATUS_BUSY_BIT_MASK))
        return true;

    unsigned long endTime = millis() + (unsigned long)timeout_lep;
    while ((status & STATUS_BUSY_BIT_MASK) && (timeout_lep <= 0 || millis() < endTime)) {
        if (readRegister(STATUS_REG, &status))
            return false;
    }
    if (!(status & STATUS_BUSY_BIT_MASK))
        return true;
    else {
        errorlepton = LEP_TIMEOUT_ERROR;
        return false;
    }
}

bool waitCommandFinish(int timeout_lep) {
    uint16_t status;
    if (readRegister(STATUS_REG, &status))
        return false;

    if (!(status & STATUS_BUSY_BIT_MASK)) {
        errorlepton = (byte)((status & STATUS_ERROR_CODE_BIT_MASK) >> STATUS_ERROR_CODE_BIT_SHIFT);
        return true;
    }

    unsigned long endTime = millis() + (unsigned long)timeout_lep;

    while ((status & STATUS_BUSY_BIT_MASK) && (timeout_lep <= 0 || millis() < endTime)) {
        if (readRegister(STATUS_REG, &status))
            return false;
    }

    if (!(status & STATUS_BUSY_BIT_MASK)) {
        errorlepton = (byte)((status & STATUS_ERROR_CODE_BIT_MASK) >> STATUS_ERROR_CODE_BIT_SHIFT);
        return true;
    }
    else {
        errorlepton = LEP_TIMEOUT_ERROR;
        return false;
    }
}







uint16_t cmdCode(uint16_t cmdID, uint16_t cmdType) {
    return (cmdID & COMMAND_MODULE_ID_BIT_MASK) | (cmdID & COMMAND_ID_BIT_MASK) | (cmdType & COMMAND_TYPE_BIT_MASK);
}

uint16_t cmdCodeOEM(uint16_t cmdID, uint16_t cmdType) {
    return (OEM_PROTECTION_BIT & COMMAND_PROTECTION_BIT_MASK) | (cmdID & COMMAND_MODULE_ID_BIT_MASK) | (cmdID & COMMAND_ID_BIT_MASK) | (cmdType & COMMAND_TYPE_BIT_MASK);
}

void sendCommand(uint16_t cmdCode) {
    if (waitCommandBegin(GEN_CMD_TIMEOUT)) {
        if (writeCmdRegister(cmdCode, NULL, 0) == 0) {
            waitCommandFinish(GEN_CMD_TIMEOUT);
        }
    }
    checkForErrors();
}

void sendCommand(uint16_t cmdCode, uint16_t value) {
    if (waitCommandBegin(GEN_CMD_TIMEOUT)) {
        if (writeCmdRegister(cmdCode, &value, 1) == 0) {
            waitCommandFinish(GEN_CMD_TIMEOUT);
        }
    }
    checkForErrors();
}

void sendCommand(uint16_t cmdCode, uint32_t value) {
    if (waitCommandBegin(GEN_CMD_TIMEOUT)) {
        if (writeCmdRegister(cmdCode, (uint16_t *)&value, 2) == 0) {
            waitCommandFinish(GEN_CMD_TIMEOUT);
        }
    }
    checkForErrors();
}

void sendCommand(uint16_t cmdCode, uint16_t *dataWords, int dataLength) {
    if (waitCommandBegin(GEN_CMD_TIMEOUT)) {
        if (writeCmdRegister(cmdCode, dataWords, dataLength) == 0) {
            waitCommandFinish(GEN_CMD_TIMEOUT);
        }
    }
    checkForErrors();
}

void receiveCommand(uint16_t cmdCode, uint16_t *value) {
    if (waitCommandBegin(GEN_CMD_TIMEOUT)) {
        if (writeRegister(COMMAND_REG, cmdCode) == 0) {
            if (waitCommandFinish(GEN_CMD_TIMEOUT)) {
                readDataRegister(value, 1);
            }
        }
    }
    checkForErrors();
}

void receiveCommand(uint16_t cmdCode, uint32_t *value) {
    if (waitCommandBegin(GEN_CMD_TIMEOUT)) {
        if (writeRegister(COMMAND_REG, cmdCode) == 0) {
            if (waitCommandFinish(GEN_CMD_TIMEOUT)) {
                readDataRegister((uint16_t *)value, 2);
            }
        }
    }
    checkForErrors();
}

void receiveCommand(uint16_t cmdCode, uint16_t *readWords, int maxLength) {
    if (waitCommandBegin(GEN_CMD_TIMEOUT)) {
        if (writeRegister(COMMAND_REG, cmdCode) == 0) {
            if (waitCommandFinish(GEN_CMD_TIMEOUT)) {
                readDataRegister(readWords, maxLength);
            }
        }
    }
    checkForErrors();
}







