﻿//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "../Configuration.h"

#include <Windows.h>
#include <IPHlpApi.h>

namespace ms {
    class HardwareInfo {
    public:
        HardwareInfo() {
            size_t size = 18;

            // Hard Drive VolumeSerialNumber
            auto volumeSerialNumber = static_cast<char*>(malloc(size));

            TCHAR szVolume[MAX_PATH + 1];
            TCHAR szFileSystem[MAX_PATH + 1];

            DWORD dwSerialNumber, dwMaxLen, dwSystemFlags;

            TCHAR szDrives[MAX_PATH + 1];
            DWORD dwLen = GetLogicalDriveStrings(MAX_PATH, szDrives);
            TCHAR* pLetter = szDrives;

            BOOL bSuccess;

            bSuccess = GetVolumeInformation(pLetter, szVolume, MAX_PATH, &dwSerialNumber, &dwMaxLen, &dwSystemFlags,
                                            szFileSystem, MAX_PATH);

            if (bSuccess) {
                sprintf_s(volumeSerialNumber, size, "%X%X", HIWORD(dwSerialNumber), LOWORD(dwSerialNumber));
            } else {
                printf("Cannot retrieve Volume information for %s\n", pLetter);
                free(volumeSerialNumber);
                return;
            }

            // HWID/MACS
            PIP_ADAPTER_INFO AdapterInfo;
            DWORD dwBufLen = sizeof(IP_ADAPTER_INFO);
            auto hwid = static_cast<char*>(malloc(size));
            auto macs = static_cast<char*>(malloc(size));

            AdapterInfo = static_cast<IP_ADAPTER_INFO*>(malloc(sizeof(IP_ADAPTER_INFO)));

            if (AdapterInfo == nullptr) {
                printf("Error allocating memory needed to call GetAdaptersinfo\n");
                free(volumeSerialNumber);
                free(hwid);
                free(macs);
                return;
            }

            // Make an initial call to GetAdaptersInfo to get the necessary size into the dwBufLen variable
            if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == ERROR_BUFFER_OVERFLOW) {
                free(AdapterInfo);
                AdapterInfo = static_cast<IP_ADAPTER_INFO*>(malloc(dwBufLen));

                if (AdapterInfo == nullptr) {
                    printf("Error allocating memory needed to call GetAdaptersinfo\n");
                    free(volumeSerialNumber);
                    free(hwid);
                    free(macs);
                    return;
                }
            }

            if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == NO_ERROR) {
                // Contains pointer to current adapter info
                PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;

                // Technically should look at pAdapterInfo->AddressLength and not assume it is 6
                sprintf_s(hwid, size, "%02X%02X%02X%02X%02X%02X",
                          pAdapterInfo->Address[0], pAdapterInfo->Address[1],
                          pAdapterInfo->Address[2], pAdapterInfo->Address[3],
                          pAdapterInfo->Address[4], pAdapterInfo->Address[5]);

                Configuration::get().set_hwid(hwid, volumeSerialNumber);

                pAdapterInfo = pAdapterInfo->Next;

                if (pAdapterInfo) {
                    // Technically should look at pAdapterInfo->AddressLength and not assume it is 6
                    sprintf_s(macs, size, "%02X-%02X-%02X-%02X-%02X-%02X",
                              pAdapterInfo->Address[0], pAdapterInfo->Address[1],
                              pAdapterInfo->Address[2], pAdapterInfo->Address[3],
                              pAdapterInfo->Address[4], pAdapterInfo->Address[5]);

                    Configuration::get().set_macs(macs);
                }
            }

            free(AdapterInfo);
            free(volumeSerialNumber);
            free(hwid);
            free(macs);
        }
    };
}
