//////////////////////////////////////////////////////////////////////////////////
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
#include "ExpTable.h"

namespace ms {
    const std::array<int64_t, ExpTable::LEVELCAP> ExpTable::values =
    {
        1, 15, 34, 57, 92, 135, 372, 560, 840, 1242, // Levels   1 -  10
        1144, 1573, 2144, 2800, 3640, 4700, 5893, 7360, 9144, 11120, // Levels  11 -  20
        13477, 16268, 19320, 22880, 27008, 31477, 36600, 42444, 48720, 55813, // Levels  21 -  30
        63800, 86784, 98208, 110932, 124432, 139372, 155865, 173280, 192400, 213345, // Levels  31 -  40
        235372, 259392, 285532, 312928, 342624, 374760, 408336, 445544, 483532, 524160, // Levels  41 -  50
        567772, 598886, 631704, 666321, 702836, 741351, 781976, 824828, 870028, 917625, // Levels  51 -  60
        967995, 1021041, 1076994, 1136013, 1198266, 1263930, 1333194, 1406252, 1483314, 1564600, // Levels  61 -  70
        1650340, 1740778, 1836173, 1936794, 2042930, 2154882, 2272970, 2397528, 2528912, 2667496, // Levels  71 -  80
        2813674, 2967863, 3130502, 3302053, 3483005, 3673873, 3875201, 4087562, 4311559, 4547832, // Levels  81 -  90
        4797053, 5059931, 5337215, 5629694, 5938202, 6263614, 6606860, 6968915, 7350811, 7753635, // Levels  91 - 100
        8178534, 8626718, 9099462, 9598112, 10124088, 10678888, 11264090, 11881362, 12532461, 13219239,
        // Levels 101 - 110
        13943653, 14707765, 15513750, 16363902, 17260644, 18206527, 19204245, 20256637, 21366700, 22537594,
        // Levels 111 - 120
        23772654, 25075395, 26449526, 27898960, 29427822, 31040466, 32741483, 34535716, 36428273, 38424542,
        // Levels 121 - 130
        40530206, 42751262, 45094030, 47565183, 50171755, 52921167, 55821246, 58880250, 62106888, 65510344,
        // Levels 131 - 140
        69100311, 72887008, 76881216, 81094306, 85594273, 90225770, 95170142, 100385466, 105886589, 111689174,
        // Levels 141 - 150
        117809740, 124265714, 131075474, 138258410, 145834970, 153826726, 162256430, 171148082, 180526997, 190419876,
        // Levels 151 - 160
        200854885, 211861732, 223471711, 223471711, 248635353, 262260570, 276632449, 291791906, 307782102, 324648562,
        // Levels 161 - 170
        342439302, 361204976, 380999008, 401877754, 423900654, 447130410, 471633156, 497478653, 524740482, 553496261,
        // Levels 171 - 180
        583827855, 615821622, 649568646, 685165008, 722712050, 762316670, 804091623, 848155844, 894634784, 943660770,
        // Levels 181 - 190
        995373379, 1049919840, 1107455447, 1168144006, 1232158297, 1299680571, 1370903066, 1446028554, 1525246918,
        1608855764, // Levels 191 - 200
        1697021059 // Level  201
    };
}
