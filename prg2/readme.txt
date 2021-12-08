 prg2
// Student name: Tomi Lotila
// Student email: tomi.lotila@tuni.fi
// Student number: H274802

kaupunkiin yhdistyvät tiet säilytetään päätietorakenteessa unordered_mapsissa,
jossa avaimmena on TownID ja hakutuloksena tien pituus.

struct kaupunki_data
{
    Name nimi;
    Coord koordinaatit;
    int verot;
    std::vector<TownID> vasalllikaupungit;
    TownID isantakaupunki;
    std::unordered_map<TownID, int> naapurit; // naapurikaupungin id ja etäisyys
    jaljitus_tiedot jaljitus;
};
unordered_mapissa tie yhteydet ja teiden pituudet löydetään tehokkaasti.


