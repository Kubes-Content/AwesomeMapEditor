//////////////////////////////////////////////////////////////////////////////////
//
//
//                     d88b         888b           d888  888888888888
//                    d8888b        8888b         d8888  888
//                   d88''88b       888'8b       d8'888  888
//                  d88'  '88b      888 '8b     d8' 888  8888888
//                 d88Y8888Y88b     888  '8b   d8'  888  888
//                d88""""""""88b    888   '8b d8'   888  888
//               d88'        '88b   888    '888'    888  888
//              d88'          '88b  888     '8'     888  888888888888
//
//
// AwesomeMapEditor: A map editor for GBA Pokémon games.
// Copyright (C) 2016 Diegoisawesome, Pokedude
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
//////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////
// Include files
//
///////////////////////////////////////////////////////////
#include <AME/System/LoadedData.hpp>
#include <AME/System/Configuration.hpp>
#include <AME/Widgets/Misc/Messages.hpp>
#include <AME/Text/String.hpp>
#include <QDateTime>


namespace ame
{
    ///////////////////////////////////////////////////////////
    // Global objects definitions
    //
    ///////////////////////////////////////////////////////////
    WildPokemonTable *dat_WildPokemonTable = NULL;
    MapBankTable *dat_MapBankTable = NULL;
    MapLayoutTable *dat_MapLayoutTable = NULL;
    OverworldTable *dat_OverworldTable = NULL;
    PokemonTable *dat_PokemonTable = NULL;
    ItemTable *dat_ItemTable = NULL;
    MapNameTable *dat_MapNameTable = NULL;


    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude, Diegoisawesome
    // Last edit by:   Diegoisawesome
    // Date of edit:   7/5/2016
    //
    ///////////////////////////////////////////////////////////
    int loadAllMapData(const qboy::Rom &rom)
    {
        // First attempts to load the configuration file
        if (!Configuration::parse(rom))
            return -1;


        QTime stopWatch;
        stopWatch.start();

        // Allocates the tables on the dynamic heap
        dat_MapNameTable = new MapNameTable;
        dat_WildPokemonTable = new WildPokemonTable;
        dat_OverworldTable = new OverworldTable;
        dat_MapBankTable = new MapBankTable;
        dat_MapLayoutTable = new MapLayoutTable;
        dat_PokemonTable = new PokemonTable;
        dat_ItemTable = new ItemTable;

        // Attempts to load map names
        if(!dat_MapNameTable->read(rom, CONFIG(MapNames)))
            return -1;

        // Attempts to load the wild Pokémon
        if (!dat_WildPokemonTable->read(rom, CONFIG(WildPokemon)))
            return -1;

        // Attempts to load the Pokémon table
        if (!dat_PokemonTable->read(rom))
            return -1;

        // Attempts to load the item table
        if (!dat_ItemTable->read(rom))
            return -1;

        // Attempts to load the overworld table
        if (!dat_OverworldTable->read(rom))
            return -1;

        // Attempts to load all the map banks
        if (!dat_MapBankTable->read(rom, CONFIG(MapBanks)))
            return -1;

        // Attempts to load the map layout table
        if (!dat_MapLayoutTable->read(rom, CONFIG(MapLayouts)))
            return -1;

        // Map wild Pokémon indices to all the maps
        for (int i = 0; i < dat_WildPokemonTable->tables().size(); i++)
        {
            quint8 bank = dat_WildPokemonTable->tables().at(i)->bank();
            quint8 map  = dat_WildPokemonTable->tables().at(i)->map();
            
            if (bank < dat_MapBankTable->banks().size() &&
                map  < dat_MapBankTable->banks().at(bank)->maps().size())
            {
                dat_MapBankTable->banks()
                        [dat_WildPokemonTable->tables().at(i)->bank()]->maps()
                        [dat_WildPokemonTable->tables().at(i)->map()]->setWildTable(i);
            }
        }

        return stopWatch.elapsed();
    }


    ///////////////////////////////////////////////////////////
    // Function type:  I/O
    // Contributors:   Pokedude, Diegoisawesome
    // Last edit by:   Pokedude
    // Date of edit:   7/5/2016
    //
    ///////////////////////////////////////////////////////////
    void clearAllMapData()
    {
        if (dat_WildPokemonTable)
            delete dat_WildPokemonTable;
        if (dat_MapBankTable)
            delete dat_MapBankTable;
        if (dat_MapNameTable)
            delete dat_MapNameTable;
        if (dat_PokemonTable)
            delete dat_PokemonTable;
        if (dat_ItemTable)
            delete dat_ItemTable;
        if (dat_OverworldTable)
            delete dat_OverworldTable;

        TilesetManager::clear();
    }
}
