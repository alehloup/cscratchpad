#include <stdio.h>
#include <windows.h>
#include <time.h>
#include "ale.h"

/*
    I use a Perfect Hash of the 413 cities

    Hash formula: (ULong(name[1:9]) % 13087) % 6570)

    Then... We do a little cheat 
    With an array that has the alphabetical order of the cities hashes, and one with their names
*/
#define ncity 413
static const Int hash_order[] = {3875, 1652, 4450, 1228, 6163, 5667, 3322, 3800, 651, 731, 739, 2594, 4532, 782, 4832, 402, 2639, 2584, 1680, 4692, 331, 2815, 6090, 6218, 6492, 2968, 4320, 3477, 3748, 4736, 4963, 939, 4562, 5441, 4040, 2464, 365, 671, 4282, 329, 4340, 4789, 4620, 1394, 4362, 4892, 3062, 4591, 4985, 3067, 3039, 4482, 770, 187, 4848, 6091, 3037, 3931, 685, 938, 3506, 3561, 4724, 6418, 5792, 918, 6183, 4615, 4439, 5868, 5923, 1284, 2484, 1438, 1481, 5746, 2935, 1032, 6026, 6195, 2897, 4783, 3636, 5606, 6500, 4575, 4393, 3332, 460, 1363, 4397, 5859, 4246, 5203, 4907, 2913, 5130, 6250, 5890, 4159, 2745, 5853, 2529, 4214, 4086, 5614, 3266, 1154, 2172, 595, 4146, 2764, 6280, 1188, 4134, 6508, 1607, 5906, 2395, 634, 1964, 4353, 1470, 89, 962, 5939, 4943, 3660, 582, 1754, 3373, 5193, 6410, 5912, 5888, 4375, 3149, 2470, 1391, 3524, 2144, 4101, 1659, 2215, 4232, 2038, 1883, 2322, 1932, 5477, 5149, 4350, 2264, 1675, 5161, 3399, 2125, 838, 1865, 4369, 1636, 5681, 5404, 3598, 1920, 1223, 1808, 2932, 2357, 4258, 2542, 700, 5969, 2171, 5324, 5414, 3313, 6225, 4052, 3280, 5818, 6520, 2530, 129, 710, 5514, 5605, 4920, 6475, 2540, 6490, 4656, 1518, 2309, 6135, 6424, 4458, 1915, 3569, 6262, 855, 6342, 4029, 6539, 5424, 5786, 3905, 5366, 1858, 1991, 258, 3316, 5627, 6172, 1486, 1579, 6321, 426, 4559, 2054, 3620, 5494, 4220, 5134, 173, 4772, 893, 5783, 930, 598, 2924, 6318, 305, 3055, 3121, 5092, 4416, 2460, 4598, 3007, 697, 2880, 2250, 531, 6471, 2284, 1003, 2223, 2773, 6, 977, 4370, 1777, 3774, 3900, 701, 2361, 4750, 593, 1396, 586, 2716, 5321, 5666, 3352, 4111, 2980, 2293, 2986, 693, 4548, 2615, 1368, 2838, 1714, 1856, 4457, 113, 2386, 5184, 1390, 2048, 321, 5597, 2501, 4360, 5581, 4976, 5925, 1093, 2626, 2906, 551, 284, 3164, 4636, 6449, 3785, 1221, 5596, 706, 588, 658, 5142, 6023, 1211, 522, 2256, 4266, 4000, 4512, 4135, 1960, 4326, 1511, 6472, 5409, 2220, 5111, 5373, 2248, 928, 3889, 6346, 5658, 4195, 5272, 202, 2877, 5957, 364, 1523, 4900, 1688, 5637, 2583, 69, 40, 6271, 1967, 1430, 1981, 734, 2288, 602, 1509, 3782, 1789, 2861, 2466, 2860, 6176, 4213, 2237, 5285, 6191, 4286, 6032, 2362, 2567, 3358, 3610, 4680, 6329, 2821, 2232, 4328, 6506, 6330, 6290, 4746, 26, 2654, 3173, 4873, 4227, 3693, 4689, 899, 2180, 718, 2758, 4600, 4880, 4872, 476, 5220, 630, 4841, 5578, 3765, 4957, 1716, 5330, 5292, 103, 1650, 495, 191, 5482, 68, 2008, 2033, 5351, 2709, 4961, 4571, 4290, 6116, 3909, 1556, 1482, 185};
static const Ccstr city_names[] = {"Abha", "Abidjan", "Abéché", "Accra", "Addis Ababa", "Adelaide", "Aden", "Ahvaz", "Albuquerque", "Alexandra", "Alexandria", "Algiers", "Alice Springs", "Almaty", "Amsterdam", "Anadyr", "Anchorage", "Andorra la Vella", "Ankara", "Antananarivo", "Antsiranana", "Arkhangelsk", "Ashgabat", "Asmara", "Assab", "Astana", "Athens", "Atlanta", "Auckland", "Austin", "Baghdad", "Baguio", "Baku", "Baltimore", "Bamako", "Bangkok", "Bangui", "Banjul", "Barcelona", "Bata", "Batumi", "Beijing", "Beirut", "Belgrade", "Belize City", "Benghazi", "Bergen", "Berlin", "Bilbao", "Birao", "Bishkek", "Bissau", "Blantyre", "Bloemfontein", "Boise", "Bordeaux", "Bosaso", "Boston", "Bouaké", "Bratislava", "Brazzaville", "Bridgetown", "Brisbane", "Brussels", "Bucharest", "Budapest", "Bujumbura", "Bulawayo", "Burnie", "Busan", "Cabo San Lucas", "Cairns", "Cairo", "Calgary", "Canberra", "Cape Town", "Changsha", "Charlotte", "Chiang Mai", "Chicago", "Chihuahua", "Chittagong", "Chișinău", "Chongqing", "Christchurch", "City of San Marino", "Colombo", "Columbus", "Conakry", "Copenhagen", "Cotonou", "Cracow", "Da Lat", "Da Nang", "Dakar", "Dallas", "Damascus", "Dampier", "Dar es Salaam", "Darwin", "Denpasar", "Denver", "Detroit", "Dhaka", "Dikson", "Dili", "Djibouti", "Dodoma", "Dolisie", "Douala", "Dubai", "Dublin", "Dunedin", "Durban", "Dushanbe", "Edinburgh", "Edmonton", "El Paso", "Entebbe", "Erbil", "Erzurum", "Fairbanks", "Fianarantsoa", "Flores,  Petén", "Frankfurt", "Fresno", "Fukuoka", "Gaborone", "Gabès", "Gagnoa", "Gangtok", "Garissa", "Garoua", "George Town", "Ghanzi", "Gjoa Haven", "Guadalajara", "Guangzhou", "Guatemala City", "Halifax", "Hamburg", "Hamilton", "Hanga Roa", "Hanoi", "Harare", "Harbin", "Hargeisa", "Hat Yai", "Havana", "Helsinki", "Heraklion", "Hiroshima", "Ho Chi Minh City", "Hobart", "Hong Kong", "Honiara", "Honolulu", "Houston", "Ifrane", "Indianapolis", "Iqaluit", "Irkutsk", "Istanbul", "Jacksonville", "Jakarta", "Jayapura", "Jerusalem", "Johannesburg", "Jos", "Juba", "Kabul", "Kampala", "Kandi", "Kankan", "Kano", "Kansas City", "Karachi", "Karonga", "Kathmandu", "Khartoum", "Kingston", "Kinshasa", "Kolkata", "Kuala Lumpur", "Kumasi", "Kunming", "Kuopio", "Kuwait City", "Kyiv", "Kyoto", "La Ceiba", "La Paz", "Lagos", "Lahore", "Lake Havasu City", "Lake Tekapo", "Las Palmas de Gran Canaria", "Las Vegas", "Launceston", "Lhasa", "Libreville", "Lisbon", "Livingstone", "Ljubljana", "Lodwar", "Lomé", "London", "Los Angeles", "Louisville", "Luanda", "Lubumbashi", "Lusaka", "Luxembourg City", "Lviv", "Lyon", "Madrid", "Mahajanga", "Makassar", "Makurdi", "Malabo", "Malé", "Managua", "Manama", "Mandalay", "Mango", "Manila", "Maputo", "Marrakesh", "Marseille", "Maun", "Medan", "Mek'ele", "Melbourne", "Memphis", "Mexicali", "Mexico City", "Miami", "Milan", "Milwaukee", "Minneapolis", "Minsk", "Mogadishu", "Mombasa", "Monaco", "Moncton", "Monterrey", "Montreal", "Moscow", "Mumbai", "Murmansk", "Muscat", "Mzuzu", "N'Djamena", "Naha", "Nairobi", "Nakhon Ratchasima", "Napier", "Napoli", "Nashville", "Nassau", "Ndola", "New Delhi", "New Orleans", "New York City", "Ngaoundéré", "Niamey", "Nicosia", "Niigata", "Nouadhibou", "Nouakchott", "Novosibirsk", "Nuuk", "Odesa", "Odienné", "Oklahoma City", "Omaha", "Oranjestad", "Oslo", "Ottawa", "Ouagadougou", "Ouahigouya", "Ouarzazate", "Oulu", "Palembang", "Palermo", "Palm Springs", "Palmerston North", "Panama City", "Parakou", "Paris", "Perth", "Petropavlovsk-Kamchatsky", "Philadelphia", "Phnom Penh", "Phoenix", "Pittsburgh", "Podgorica", "Pointe-Noire", "Pontianak", "Port Moresby", "Port Sudan", "Port Vila", "Port-Gentil", "Portland (OR)", "Porto", "Prague", "Praia", "Pretoria", "Pyongyang", "Rabat", "Rangpur", "Reggane", "Reykjavík", "Riga", "Riyadh", "Rome", "Roseau", "Rostov-on-Don", "Sacramento", "Saint Petersburg", "Saint-Pierre", "Salt Lake City", "San Antonio", "San Diego", "San Francisco", "San Jose", "San José", "San Juan", "San Salvador", "Sana'a", "Santo Domingo", "Sapporo", "Sarajevo", "Saskatoon", "Seattle", "Seoul", "Seville", "Shanghai", "Singapore", "Skopje", "Sochi", "Sofia", "Sokoto", "Split", "St. John's", "St. Louis", "Stockholm", "Surabaya", "Suva", "Suwałki", "Sydney", "Ségou", "Tabora", "Tabriz", "Taipei", "Tallinn", "Tamale", "Tamanrasset", "Tampa", "Tashkent", "Tauranga", "Tbilisi", "Tegucigalpa", "Tehran", "Tel Aviv", "Thessaloniki", "Thiès", "Tijuana", "Timbuktu", "Tirana", "Toamasina", "Tokyo", "Toliara", "Toluca", "Toronto", "Tripoli", "Tromsø", "Tucson", "Tunis", "Ulaanbaatar", "Upington", "Vaduz", "Valencia", "Valletta", "Vancouver", "Veracruz", "Vienna", "Vientiane", "Villahermosa", "Vilnius", "Virginia Beach", "Vladivostok", "Warsaw", "Washington, D.C.", "Wau", "Wellington", "Whitehorse", "Wichita", "Willemstad", "Winnipeg", "Wrocław", "Xi'an", "Yakutsk", "Yangon", "Yaoundé", "Yellowknife", "Yerevan", "Yinchuan", "Zagreb", "Zanzibar City", "Zürich", "Ürümqi", "İzmir"};

typedef struct City { Cstr name; Long count; Long sum; Long min; Long max; } City;

_proc run(void) {
    static City cities[6570] = ZERO_INIT;
    for (Int i = 0; i < ncity; ++i) {
        City *city = &cities[hash_order[i]];
        city->name = city_names[i];
        city->min = 1000;
    }

    Mmap mmap_info = mmap_open(S("C:/Users/Aleh/1brc_java/measurements1b.txt")); 
    {
        Buffer contents = mmap_info.contents;
        Long len = contents.len;
        Byte *data = contents.elements;

        ULong city_hash = 0, sum_hash = 0;

        for (Long i = 1; i < len; ++i /* SKIPS THE FIRST LETTER OF THE LINE*/) {
            // Each iteration we start at the second letter of a line

            sum_hash = 0;
            city_hash = 0;
            
            for (Int si = 0; si < 8 and data[i] != ';'; ++si, ++i) {
                city_hash = sum_hash + (ULong)data[i];
                sum_hash = city_hash << 8;
            }
            city_hash = ((city_hash % 13087) % 6570);

            while(data[i] != ';') {
                ++i;
            }
            ++i;

            //number start
            Long measure = 1;
            if (data[i] == '-') {
                measure = -1;
                ++i;
            }
            
            if (data[i+1] == '.') {
                measure *= (data[i] - '0')*10 + (data[i+2] - '0');
                i += 4; // skips to next line
            } else {
                measure *= (data[i] - '0')*100 + (data[i+1] - '0')*10 + (data[i+3] - '0');
                i += 5; // skips to next line
            }

            City *city = &cities[city_hash];
            ++city->count;
            city->sum += measure;
            
            if (measure < city->min) {
                city->min = measure;
            }
            if (measure > city->max) {
                city->max = measure;
            }
        }
    }
    mmap_close(mmap_info);

    for (Int i = 0; i < ncity; ++i) {
        City *city = &cities[hash_order[i]];
        printf("%s  #%lld  $%.1f  @%.1f  [%.1f  %.1f]\n", city->name, city->count, 
            /*$sum*/ (double)city->sum / 10.0,
            /*@avg*/ ((double)city->sum / ((double)city->count*10)),
            /*[min  max]*/ (double)city->min/10.0, (double)city->max/10.0
        );
        break;
    }
}

int main(void) {

    START_WATCH;
    run();
    STOP_WATCH;

    return 0;
}
