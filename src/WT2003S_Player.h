/**
    The MIT License (MIT)

    Author: Baozhu Zuo (baozhu.zuo@gmail.com)

    Copyright (C) 2019  Seeed Technology Co.,Ltd.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/
#ifndef __WT2003S__
#define __WT2003S__

#include "Arduino.h"



#define WT2003S_NUM_CMD_BYTES 11
#define WT2003S_NUM_NAME_BYTES 9

//These are the commands that are sent over serial to the WT2003S
#define WT2003S_SPIFLASH_PLAY_INDEX 0xA0
#define WT2003S_SD_PLAY_INDEX_IN_ROOT 0xA2
#define WT2003S_SD_PLAY_FILE_IN_ROOT 0xA3
#define WT2003S_SD_PLAY_INDEX_IN_FOLDER 0xA4
#define WT2003S_UDISK_PLAY_INDEX_IN_ROOT 0xA6
#define WT2003S_UDISK_PLAY_FILE_IN_ROOT 0xA7
#define WT2003S_UDISK_PLAY_INDEX_IN_FOLDER 0xA8
#define WT2003S_PAUSE_OR_PLAY 0xAA
#define WT2003S_STOP 0xAB
#define WT2003S_NEXT 0xAC
#define WT2003S_PREVIOUS 0xAD
#define WT2003S_SET_VOLUME 0xAE //Can take more than 150ms to complete
#define WT2003S_SET_PLAYMODE 0xAF
#define WT2003S_SET_PLAYMODE_SINGLE_NO_LOOP 0x00
#define WT2003S_SET_PLAYMODE_SINGLE_LOOP 0x01
#define WT2003S_SET_PLAYMODE_ALL_LOOP 0x02
#define WT2003S_SET_PLAYMODE_RANDOM 0x03
#define WT2003S_SET_CUTIN_MODE 0xB1
#define WT2003S_COPY_SDTOSPIFLASH 0xB3
#define WT2003S_COPY_UDISKTOSPIFLASH 0xB4
#define WT2003S_STORY_USERDATA 0xB8
#define WT2003S_ISNEED_RETURNCODE 0xBA
#define WT2003S_SWITCH_WORKDATA 0xD2

#define WT2003S_GET_VOLUME 0xC1
#define WT2003S_GET_STATE 0xC2
#define WT2003S_GET_SPIFLASH_SONGCOUNT 0xC3
#define WT2003S_GET_SD_SONGCOUNT 0xC5
#define WT2003S_GET_SD_SONGS_IN_FOLDER_COUNT 0xC6
#define WT2003S_GET_UDISK_SONGCOUNT 0xC7
#define WT2003S_GET_UDISK_SONGS_IN_FOLDER_COUNT 0xC8


#define WT2003S_GET_FILE_PLAYING 0xC9
#define WT2003S_DISKSTATUS 0xCA
#define WT2003S_GET_SONG_NAME_PLAYING 0xCB
#define WT2003S_GET_USERDATA 0xCF

#define WT2003S_START_CODE 0x7E
#define WT2003S_END_CODE 0xEF
#define WT2003S_NUM_CMD_BYTES 11

#define WT2003S_MAX_VOLUME 0x1F
#define WT2003S_MIN_VOLUME 0x00
#define WT2003S_TIMEOUT 1000
typedef enum {
    SINGLE_SHOT    = 0x00,
    SINGLE_CYCLE   = 0x01,
    CYCLE    = 0x02,
    RANDOM   = 0x03,
} PLAY_MODE;

typedef enum {
    SPIFLASH    = 0x00,
    SD   = 0x01,
    UDISK    = 0x02,
} STROAGE;


template <class T>
class WT2003S {
  private:
    T* _serial;
    uint8_t _busyPin;
    uint8_t sendCommand(uint8_t commandLength, uint8_t* data, uint8_t len);
    uint8_t commandBytes[WT2003S_NUM_CMD_BYTES];

  public:
    WT2003S();
    void init(T& serialPort);
    void init(T& serialPort, uint8_t pin);

    uint8_t playSPIFlashSong(uint16_t index);
    uint8_t playSDRootSong(uint32_t index);
    uint8_t playSDSong(const char* fileName);
    uint8_t playSDDirectorySong(const char* dir, uint16_t index);
    uint8_t playUDiskRootSong(uint32_t index);
    uint8_t playUDiskSong(const char* fileName);
    uint8_t playUDiskDirectorySong(const char* dir, uint32_t index);
    uint8_t pause_or_play();
    uint8_t stop();
    uint8_t next();
    uint8_t previous();
    uint8_t volume(uint8_t vol);
    uint8_t volumeDown();
    uint8_t volumeUp();
    uint8_t playMode(PLAY_MODE mode);
    uint8_t cutInPlay(STROAGE device, uint32_t index);
    uint8_t copySDtoSPIFlash();
    uint8_t copyUDisktoSPIFlash();
    uint8_t writeUserData(uint16_t address,  uint32_t data);
    uint8_t switchWorkDisk(STROAGE disk);

    int8_t getVolume();
    int8_t getStatus();
    uint32_t getSPIFlashMp3FileNumber();
    uint32_t getSDMp3FileNumber();
    uint32_t getSDDirectoryMp3FileNumber(const char* dir);
    uint32_t getUDiskMp3FileNumber();
    uint32_t getUDiskDirectoryMp3FileNumber(const char* dir);
    uint32_t getTracks();
    void getSongName(char* Songname);
    uint8_t getDiskStatus();
    void getSPIFLashMp3Data(char* data, uint16_t address, uint16_t len);
};

template <class T>
WT2003S<T>::WT2003S() {
    _busyPin = -1;
}

template <class T>
void WT2003S<T>::init(T& serialPort) {
    _serial = &serialPort;
}

template <class T>
void WT2003S<T>::init(T& serialPort, uint8_t pin) {
    _serial = &serialPort;
    _busyPin = pin;
}


/****************************************************************
    Function Name: sendCommand
    Description: send command  to WT2003S
    Parameters: commandLength:uint8_t, *data:uint8_t*, len:uint8_t
    Return: 0:success -1:fail
****************************************************************/
template <class T>
uint8_t WT2003S<T>::sendCommand(uint8_t commandLength, uint8_t* data, uint8_t len) {
    long time;
    bool is_again = true;
    int  again_count = 0;
again:
    //Clear anything in the buffer
    while (_serial->available() > 0) {
        _serial->read();
    }
    _serial->write(WT2003S_START_CODE);
    _serial->write(commandLength + 2); //Add one byte for 'length', one for CRC

    //Begin sending command bytes while calc'ing CRC
    byte crc = commandLength + 2;
    for (byte x = 0; x < commandLength; x++) { //Length + command code + parameter
        _serial->write(commandBytes[x]); //Send this byte
        crc += commandBytes[x];          //Add this byte to the CRC
    }

    _serial->write(crc); //Send CRC
    _serial->write(WT2003S_END_CODE);

    time = millis();
    if (data == NULL) {
        while (! _serial->available() && ((millis() - time) < WT2003S_TIMEOUT)) {}
        return _serial->read();
    } else {
        time = millis();
        while ((millis() - time) < WT2003S_TIMEOUT) {
            if (_serial->available()) {
                if (commandBytes[0] == _serial->read()) {
                    is_again = false;
                    break;
                }
            }
        }
        if (is_again) {
            if (++again_count == 5) {
                return -1;
            }
            Serial.println(String(commandBytes[0], HEX) + "again");
            goto again;
        }
        if (len > 9) {
            len = 9;
        }
        for (int i = 1; i < len; i++) {
            time = millis();
            while (! _serial->available() && ((millis() - time) < WT2003S_TIMEOUT)) {}
            data[i - 1] = _serial->read();
        }
    }
    return 0;
}

/****************************************************************
    Function Name: playSPIFlashSong
    Description: Specify the music index to play by SPI, the index is decided by the input sequence of the music.
    Parameters: index: the music index: 0-65535.
    Return: 0:success -1:fail
****************************************************************/
template <class T>
uint8_t WT2003S<T>::playSPIFlashSong(uint16_t index) {
    commandBytes[0] = WT2003S_SPIFLASH_PLAY_INDEX;
    commandBytes[1] = (index >> 8) & 0xff;
    commandBytes[2] = 0xff & index;
    return sendCommand(3, NULL, 0);
}

/****************************************************************
    Function Name: playSDRootSong
    Description: Specify the music index to play in SD Card root directory, the index is decided by the input sequence of the music.
    Parameters: index: the music index: 0-65535.
    Return: 0:success -1:fail
****************************************************************/
template <class T>
uint8_t WT2003S<T>::playSDRootSong(uint32_t index) {
    commandBytes[0] = WT2003S_SD_PLAY_INDEX_IN_ROOT;
    commandBytes[1] = (index >> 8) & 0xff;
    commandBytes[2] = 0xff & index;
    return sendCommand(3, NULL, 0);
}

/****************************************************************
    Function Name: playSDSong
    Description: Specify the music name to play in SD Card
    Parameters: fileName: the music name: string
    Return: 0:success -1:fail
****************************************************************/
template <class T>
uint8_t WT2003S<T>::playSDSong(const char* fileName) {
    commandBytes[0] = WT2003S_SD_PLAY_FILE_IN_ROOT;
    commandBytes[1] = fileName[0];
    commandBytes[2] = fileName[1];
    commandBytes[3] = fileName[2];
    commandBytes[4] = fileName[3];
    return sendCommand(5, NULL, 0);
}

/****************************************************************
    Function Name: playSDDirectorySong
    Description: Specify the directory and music index to play in SD Card.
    Parameters: dir:directory name, index: the music index: 0-65535.
    Return: 0:success -1:fail
****************************************************************/
template <class T>
uint8_t WT2003S<T>::playSDDirectorySong(const char* dir, uint16_t index) {
    commandBytes[0] = WT2003S_SD_PLAY_INDEX_IN_FOLDER;
    commandBytes[1] = dir[0];
    commandBytes[2] = dir[1];
    commandBytes[3] = dir[2];
    commandBytes[4] = dir[3];
    commandBytes[5] = dir[4];
    commandBytes[6] = (index >> 8) & 0xff;
    commandBytes[7] = 0xff & index;
    return sendCommand(8, NULL, 0);
}

/****************************************************************
    Function Name: playUDiskRootSong
    Description: Specify the music index to play in UDISK root directory, the index is decided by the input sequence of the music.
    Parameters: index: the music index: 0-65535.
    Return: 0:success -1:fail
****************************************************************/
template <class T>
uint8_t WT2003S<T>::playUDiskRootSong(uint32_t index) {
    commandBytes[0] = WT2003S_UDISK_PLAY_INDEX_IN_ROOT;
    commandBytes[1] = (index >> 8) & 0xff;
    commandBytes[2] = 0xff & index;
    return sendCommand(3, NULL, 0);
}

/****************************************************************
    Function Name: playUDiskSong
    Description: Specify the music name to play in UDISK.
    Parameters: fileName: the music name: string
    Return: 0:success -1:fail
****************************************************************/
template <class T>
uint8_t WT2003S<T>::playUDiskSong(const char* fileName) {
    commandBytes[0] = WT2003S_UDISK_PLAY_FILE_IN_ROOT;
    commandBytes[1] = fileName[0];
    commandBytes[2] = fileName[1];
    commandBytes[3] = fileName[2];
    commandBytes[4] = fileName[3];
    return sendCommand(5, NULL, 0);
}

/****************************************************************
    Function Name: playSDDirectorySong
    Description: Specify the directory and music index to play in UDISK.
    Parameters: dir:directory name, index: the music index: 0-65535.
    Return: 0:success -1:fail
****************************************************************/
template <class T>
uint8_t WT2003S<T>::playUDiskDirectorySong(const char* dir, uint32_t index) {
    commandBytes[0] = WT2003S_UDISK_PLAY_INDEX_IN_FOLDER;
    commandBytes[1] = dir[0];
    commandBytes[2] = dir[1];
    commandBytes[3] = dir[2];
    commandBytes[4] = dir[3];
    commandBytes[5] = dir[4];
    commandBytes[6] = (index >> 8) & 0xff;
    commandBytes[7] = 0xff & index;
    return sendCommand(8, NULL, 0);
}

/****************************************************************
    Function Name: pause_or_play
    Description: Pause or play the current music.
    Parameters: none
    Return: 0:success -1:fail
****************************************************************/
template <class T>
uint8_t WT2003S<T>::pause_or_play() {
    commandBytes[0] = WT2003S_PAUSE_OR_PLAY;
    return sendCommand(1, NULL, 0);
}

/****************************************************************
    Function Name: pause_or_play
    Description: stop the current music.
    Parameters: none
    Return: 0:success -1:fail
****************************************************************/
template <class T>
uint8_t WT2003S<T>::stop() {
    commandBytes[0] = WT2003S_STOP;
    return sendCommand(1, NULL, 0);
}

/****************************************************************
    Function Name: next
    Description: play the next track music.
    Parameters: none
    Return: 0:success -1:fail
****************************************************************/
template <class T>
uint8_t WT2003S<T>::next() {
    commandBytes[0] = WT2003S_NEXT;
    return sendCommand(1, NULL, 0);
}

/****************************************************************
    Function Name: previous
    Description: play the pervious track music.
    Parameters: none
    Return: 0:success -1:fail
****************************************************************/
template <class T>
uint8_t WT2003S<T>::previous() {
    commandBytes[0] = WT2003S_PREVIOUS;
    return sendCommand(1, NULL, 0);
}

/****************************************************************
    Function Name: volume
    Description: set the volume
    Parameters: vol:the value of volume 0-30
    Return: 0:success -1:fail
****************************************************************/
template <class T>
uint8_t WT2003S<T>::volume(uint8_t vol) {
    if (vol > WT2003S_MAX_VOLUME) {
        vol = WT2003S_MAX_VOLUME;
    }
    commandBytes[0] = WT2003S_SET_VOLUME;
    commandBytes[1] = vol;
    return sendCommand(2, NULL, 0);
}

/****************************************************************
    Function Name: volumeDown
    Description: turn down the volume.
    Parameters: none
    Return: 0:success -1:fail
****************************************************************/
template <class T>
uint8_t WT2003S<T>::volumeDown() {
    uint8_t vol = this->getVolume();
    return this->volume(++vol);
}

/****************************************************************
    Function Name: volumeUp
    Description: turn up the volume.
    Parameters: none
    Return: 0:success -1:fail
****************************************************************/
template <class T>
uint8_t WT2003S<T>::volumeUp() {
    uint8_t vol = this->getVolume();
    if (--vol > 31) {
        vol = 0;
    }
    return this->volume(vol);

}

/****************************************************************
    Function Name: playMode
    Description: select the playMode
    Parameters:  mode:SINGLE_SHOT,SINGLE_CYCLE,CYCLE,RANDOM
    Return: 0:success -1:fail
****************************************************************/
template <class T>
uint8_t WT2003S<T>::playMode(PLAY_MODE mode) {
    commandBytes[0] = WT2003S_SET_PLAYMODE;
    commandBytes[1] = mode;
    return sendCommand(2, NULL, 0);
}

/****************************************************************
    Function Name: cutInPlay
    Description: cutInPlay
    Parameters: device:UDISK,SD,SPI, index:0-65535
    Return: 0:success -1:fail
    Note: if current playing in spi flash ,this API cannt be used
****************************************************************/
template <class T>
uint8_t WT2003S<T>::cutInPlay(STROAGE device, uint32_t index) {
    commandBytes[0] = WT2003S_SET_CUTIN_MODE;
    commandBytes[1] = device;
    commandBytes[2] = index;
    return sendCommand(3, NULL, 0);
}

/****************************************************************
    Function Name: copySDtoSPIFlash
    Description: Copy SD to SPI-FLASH
    Parameters: none
    Return:    0x00, mp3 file copy ok, config data copy ok
              0x01, mp3 file copy ok, config  data error
              0x02, mp3 file copy error. config data ok , maybe mp3 file too big to copy
              0x03, mp3 file cope ok, config data error, maybe mp3 file too big to copy
****************************************************************/
template <class T>
uint8_t WT2003S<T>::copySDtoSPIFlash() {
    commandBytes[0] = WT2003S_COPY_SDTOSPIFLASH;
    return sendCommand(1, NULL, 0);
}

/****************************************************************
    Function Name: copyUDisktoSPIFlash
    Description: Copy UDISK to SPI-FLASH
    Parameters: none
    Return:    0x00, mp3 file copy ok, config data copy ok
              0x01, mp3 file copy ok, config  data error
              0x02, mp3 file copy error. config data ok , maybe mp3 file too big to copy
              0x03, mp3 file cope ok, config data error, maybe mp3 file too big to copy
****************************************************************/
template <class T>
uint8_t WT2003S<T>::copyUDisktoSPIFlash() {
    commandBytes[0] = WT2003S_COPY_UDISKTOSPIFLASH;
    return sendCommand(1, NULL, 0);
}

template <class T>
uint8_t WT2003S<T>::writeUserData(uint16_t address, uint32_t data) {
    commandBytes[0] = WT2003S_STORY_USERDATA;
    commandBytes[1] = (address >> 8) & 0xff;
    commandBytes[2] = 0xff & address;
    commandBytes[3] = (data >> 24) & 0xff;
    commandBytes[4] = (data >> 16) & 0xff;
    commandBytes[5] = (data >> 8) & 0xff;
    commandBytes[6] = 0xff & data;
    return sendCommand(7, NULL, 0);
}

/****************************************************************
    Function Name: switchWorkDisk
    Description: switch the work disk
    Parameters: device:UDISK,SD,SPIFLASH
    Return: 0:success -1:fail
****************************************************************/
template <class T>
uint8_t WT2003S<T>::switchWorkDisk(STROAGE disk) {
    commandBytes[0] = WT2003S_SWITCH_WORKDATA;
    commandBytes[1] = disk;
    return sendCommand(2, NULL, 0);
}

/****************************************************************
    Function Name: getVolume
    Description: get the current volume
    Parameters: none
    Return: vol:the value of volume:0-30
****************************************************************/
template <class T>
int8_t WT2003S<T>::getVolume() {
    uint8_t vol;
    commandBytes[0] = WT2003S_GET_VOLUME;
    sendCommand(1, &vol, 2);
    return vol;
}

/****************************************************************
    Function Name: getStatus
    Description: get the current Status
    Parameters: none
    Return: status:play(01), stop(02), pause(03)
****************************************************************/
template <class T>
int8_t WT2003S<T>::getStatus() {
    uint8_t status;
    commandBytes[0] = WT2003S_GET_STATE;
    sendCommand(1, &status, 2);
    return status;
}

/****************************************************************
    Function Name: getSPIFlashMp3FileNumber
    Description: Query the total number of music files in SPIFLASH.
    Parameters: none
    Return: num:the total number of music files in SPIFLAS
****************************************************************/
template <class T>
uint32_t WT2003S<T>::getSPIFlashMp3FileNumber() {
    uint8_t num;
    commandBytes[0] = WT2003S_GET_SPIFLASH_SONGCOUNT;
    sendCommand(1, &num, 2);
    return num;
}

/****************************************************************
    Function Name: getSDMp3FileNumber
    Description: Query the total number of music files in SD.
    Parameters: none
    Return: num:the total number of music files in SD
****************************************************************/
template <class T>
uint32_t WT2003S<T>::getSDMp3FileNumber() {
    union {
        uint8_t d[2];
        uint16_t n;
    } num;
    uint8_t tmp;
    commandBytes[0] = WT2003S_GET_SD_SONGCOUNT;
    sendCommand(1, num.d, 3);
    tmp = num.d[0];
    num.d[0] = num.d[1];
    num.d[1] = tmp;
    return num.n;
}

/****************************************************************
    Function Name: getSDDirectoryMp3FileNumber
    Description: Query the total number of music files in specified directory of SD.
    Parameters: dir: specified directory name
    Return: num:the total number of music files in specified directory of SD.
****************************************************************/
template <class T>
uint32_t WT2003S<T>::getSDDirectoryMp3FileNumber(const char* dir) {
    uint8_t num;
    commandBytes[0] = WT2003S_GET_SD_SONGS_IN_FOLDER_COUNT;
    commandBytes[1] = dir[0];
    commandBytes[2] = dir[1];
    commandBytes[3] = dir[2];
    commandBytes[4] = dir[3];
    commandBytes[5] = dir[4];
    sendCommand(6, &num, 2);
    return num;
}

/****************************************************************
    Function Name: getUDiskMp3FileNumber
    Description: Query the total number of music files in UDISK.
    Parameters: none
    Return: num:the total number of music files in SD
****************************************************************/
template <class T>
uint32_t WT2003S<T>::getUDiskMp3FileNumber() {
    uint8_t num;
    commandBytes[0] = WT2003S_GET_UDISK_SONGCOUNT;
    sendCommand(1, &num, 2);
    return num;
}

/****************************************************************
    Function Name: getUDiskDirectoryMp3FileNumber
    Description: Query the total number of music files in specified directory of Disk.
    Parameters: dir: specified directory name
    Return: num:the total number of music files in specified directory of Disk.
****************************************************************/
template <class T>
uint32_t WT2003S<T>::getUDiskDirectoryMp3FileNumber(const char* dir) {
    uint8_t num;
    commandBytes[0] = WT2003S_GET_UDISK_SONGS_IN_FOLDER_COUNT;
    commandBytes[1] = dir[0];
    commandBytes[2] = dir[1];
    commandBytes[3] = dir[2];
    commandBytes[4] = dir[3];
    commandBytes[5] = dir[4];
    sendCommand(6, &num, 2);
    return num;
}


/****************************************************************
    Function Name: getTracks
    Description:Query the currently playing tracks
    Parameters: none
    Return: index:the music file index
****************************************************************/
template <class T>
uint32_t WT2003S<T>::getTracks() {
    union {
        uint8_t d[2];
        uint16_t n;
    } data;
    uint8_t tmp;
    commandBytes[0] = WT2003S_GET_FILE_PLAYING;
    sendCommand(1, data.d, 3);
    tmp = data.d[0];
    data.d[0] = data.d[1];
    data.d[1] = tmp;
    return data.n;
}

/****************************************************************
    Function Name: getSongName
    Description:Query the song name of the currently playing song
    Parameters: *Songname the pointer of Songname
    Return: none
****************************************************************/
template <class T>
void WT2003S<T>::getSongName(char* Songname) {
    commandBytes[0] = WT2003S_GET_SONG_NAME_PLAYING;
    sendCommand(1, (uint8_t*)Songname, 9);
}

/****************************************************************
    Function Name: getDiskStatus
    Description: Query the current peripheral connection status
    Parameters: none
    Return: 0X01: no PC connection (BIT3=0), no U disk (BIT2=0), no SD card (BIT1=0), spi-flash (BIT0=1);
		   0X07: no PC connection (BIT3=0), U disk (BIT2=1), SD card (BIT1=1), spi-flash (BIT0=1);
****************************************************************/
template <class T>
uint8_t WT2003S<T>::getDiskStatus() {
    uint8_t disk;
    commandBytes[0] = WT2003S_DISKSTATUS;
    sendCommand(1, &disk, 2);
    return disk;
}

/****************************************************************
    Function Name: getSPIFLashMp3Data
    Description:Read user-cached data at the specified address (CF) (just read config in Flash)
    Parameters: *data the pointer of data
    Return: none
****************************************************************/
template <class T>
void WT2003S<T>::getSPIFLashMp3Data(char* data, uint16_t address, uint16_t len) {
    if (len > 512) {
        len = 512;
    }
    commandBytes[0] = WT2003S_GET_USERDATA;
    commandBytes[1] = 0x00;
    commandBytes[2] = (address >> 8) & 0xff;
    commandBytes[3] = 0xff & address;

    commandBytes[4] = (len >> 8) & 0xff;
    commandBytes[5] = 0xff & len;
    sendCommand(6, (uint8_t*)data, len + 4);
}

#endif
