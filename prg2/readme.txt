 prg2
// Student name: Tomi Lotila
// Student email: tomi.lotila@tuni.fi
// Student number: H274802

kaupunkien tieyhteydet säilytetään päätietorakenteessa unordered_setissa,
jossa naapureiden TownID:eet.

struct kaupunki_data
{
    Name nimi;
    Coord koordinaatit;
    int verot;
    std::vector<TownID> vasalllikaupungit;
    TownID isantakaupunki;
    std::unordered_set<TownID> naapurit;
};

kaupunkien yhteyksien jäljitys dataa ei säilytetä päämuistirakenteessa, sillä ei tuhlata muistia.
