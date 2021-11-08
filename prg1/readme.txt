Tomi Lotila
prg1 dokumentaatio (datastructures luokka)

Luokan päätietorakenne:
- tietorakenteen tulee säilyttää
    - kaupungin nimi
    - id
    - koordinaatit
    - verot
    - lista vassallikaupungeista
    - isäntäkaupunki
- kaupunkeja haetaan yleisimmin id-luvulla.
- id lukuja ei ikinä haeta järjestyksessä, joten ei tarvitse pitää järjestyksessä tietorakenteessa.
- alkioita halutaan monessa eri järjestyksessä (nimen aakkos- ja etäisyysjärjestyksssä), joten
alkiot täytyy laittaa järjestykseen haun jälkeen.
- tietorakenteeksi valitaan unordered_map, jossa id on avaimmena ja oma structi kohteena.
    - unordered_map tietorakenteesta avaimmella haku on keskimäärin vakio aikainen.
    - alkion lisäys ja poisto ovat myös keskimäärin vakio aikaisia.
- Muut tiedot säilytetään structissa.
    struct
    {
    - kaupungin nimi
    - koordinaatit
    - verot
    - lista vassallikaupungeista
    - isäntäkaupunki
    }
- Vasallikaupunkilistan tietorakenne
    - halutaan usein kaupungin kaikki vasallikaupungit
    - vasallikaupunkeja ei tarvitse säilyttää järjestyksessä (ei haeta järjestyksessä)
    - vasallikaupungin lisäys on tehokasta
    - isäntäkaupungin vasalliskaupunkeja poistetaan harvoin, joten
    poiston ei tarvitse olla tehkokas.
- Vasallikaupunkilistan tietorakenteeksi valitaan vector, sillä:
    - lisäys listan perään on vakio aikainen.
    - järjestysluvulla haku on vakio aikainen.
    - tietyn vasallikaupungin haku on lineaarinen (poistoa varten).


Luokan monimutkaisimpien metodejen pseudokoodit:


Lisää tietorakenteeseen uuden kaupungin annetulla
uniikilla id:llä, nimellä, sijainnilla ja verotulolla. Aluksi
kaupungit eivät ole minkään toisen kaupungin
vasalleja. Jos annetulla id:llä on jo kaupunki, ei tehdä
mitään ja palautetaan false, muuten palautetaan
true.
bool Datastructures::add_town(TownID /*id*/, const Name &/*name*/, Coord /*coord*/, int /*tax*/)
{

}

Name Datastructures::get_town_name(TownID /*id*/)
{

}

Coord Datastructures::get_town_coordinates(TownID /*id*/)
{

}

int Datastructures::get_town_tax(TownID /*id*/)
{

}

std::vector<TownID> Datastructures::all_towns()
{

}

std::vector<TownID> Datastructures::find_towns(const Name &/*name*/)
{

}

bool Datastructures::change_town_name(TownID /*id*/, const Name &/*newname*/)
{

}

std::vector<TownID> Datastructures::towns_alphabetically()
{

}

std::vector<TownID> Datastructures::towns_distance_increasing()
{

}

TownID Datastructures::min_distance()
{

}

TownID Datastructures::max_distance()
{

}

bool Datastructures::add_vassalship(TownID /*vassalid*/, TownID /*masterid*/)
{

}

std::vector<TownID> Datastructures::get_town_vassals(TownID /*id*/)
{

}

std::vector<TownID> Datastructures::taxer_path(TownID /*id*/)
{

}

bool Datastructures::remove_town(TownID /*id*/)
{

}

std::vector<TownID> Datastructures::towns_nearest(Coord /*coord*/)
{

}

std::vector<TownID> Datastructures::longest_vassal_path(TownID /*id*/)
{

}

int Datastructures::total_net_tax(TownID /*id*/)
{

}
