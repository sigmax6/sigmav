/*
 * SOCLIB_LGPL_HEADER_BEGIN
 *
 * This file is part of SoCLib, GNU LGPLv2.1.
 *
 * SoCLib is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; version 2.1 of the License.
 *
 * SoCLib is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with SoCLib; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 * SOCLIB_LGPL_HEADER_END
 *
 * Copyright (c) UPMC, Lip6, SoC
 *         Nicolas Pouillon <nipo@ssji.net>, 2006-2007
 *
 *         Commented by sigmax6 <sigmax6@live.com>, 2011
 *
 * Maintainers: nipo
 */

#include <iostream>
#include <cstdlib>

////////////////////////////////////////////////////////////////////
// Part 1 : Include files                                         //
////////////////////////////////////////////////////////////////////

#include "mapping_table.h"

// Include cputype define
#include "mips32.h"

// Components
#include "vci_xcache_wrapper.h"
#include "vci_ram.h"
#include "vci_multi_tty.h"
#include "vci_vgmn.h"

#include "segmentation.h"

int _main(int argc, char *argv[])
{
	using namespace sc_core;
	// Avoid repeating these everywhere
	using soclib::common::IntTab;
	using soclib::common::Segment;

        ////////////////////////////////////////////////////////////////////
        // Part 2 : Mapping table                                         //
        ////////////////////////////////////////////////////////////////////

	// Define our VCI parameters
        // For all the VCI links of the platform ,defined size in bits of
        // Different fileds involved during the transfer of one common word
        // Or one response word
	typedef soclib::caba::VciParams<4,9,32,1,1,1,8,1,1,1> vci_param;
        // 10 fields to define
        // 1:  cell_size=4 *8 = 32 bits    number of cells transfered during each cycle
        // 2:  plen_size=2^9  = 512 words  min vci packet length
        // 3:  addr_size=32 bits           address size , also means the size of shared memory is MAX 4G
        // 4:  rerror_size=1 bit           set the error stat
        // 5:  clen_size=1 bit
        // 6:  rflag_size=1 bit
        // 7:  srcid_size=8 bits           means there are MAX 2^8=256 initiators
        // 8:  pktid_size=1 bit            packet identifier size 1
        // 9:  trdid_size=1 bit            thread identifier size 1


	// Mapping table

	soclib::common::MappingTable maptab(32, IntTab(8), IntTab(8), 0x00300000);
        //Building and init the mappingtable
        // 1: addr_size = 32 bits
        // 2: IntTab 8 tells the mapping table that the highest 8 bits(MSB 8) are used to
        //    decode the common packet
        // 3: the second IntTab 8 tells the mapping table that the MSB 8 of the src field
        //    is used to the address decoding of the response packet
        // 4: mask code

        // Add segments :   name   base_address  size   vci_init_idnumber cached?
	maptab.add(Segment("reset", RESET_BASE, RESET_SIZE, IntTab(0), true));
	maptab.add(Segment("excep", EXCEP_BASE, EXCEP_SIZE, IntTab(0), true));
	maptab.add(Segment("text" , TEXT_BASE , TEXT_SIZE , IntTab(0), true));
	maptab.add(Segment("data" , DATA_BASE , DATA_SIZE , IntTab(0), true));

	maptab.add(Segment("tty"  , TTY_BASE  , TTY_SIZE  , IntTab(1), false));

        //Once the maptable defined ,the mapped objects can be passed as a argument to all the socket components

        ////////////////////////////////////////////////////////////////////
        // Part 3 : signals                                               //
        ////////////////////////////////////////////////////////////////////

	sc_clock	signal_clk("signal_clk");
	sc_signal<bool> signal_resetn("signal_resetn");

        //mips's 6 interrupt ports
	sc_signal<bool> signal_mips0_it0("signal_mips0_it0");
	sc_signal<bool> signal_mips0_it1("signal_mips0_it1");
	sc_signal<bool> signal_mips0_it2("signal_mips0_it2");
	sc_signal<bool> signal_mips0_it3("signal_mips0_it3");
	sc_signal<bool> signal_mips0_it4("signal_mips0_it4");
	sc_signal<bool> signal_mips0_it5("signal_mips0_it5");

        //Tty makes a interrupt
	sc_signal<bool> signal_tty_irq0("signal_tty_irq0");

        //Connet the cache to the vgmn
	soclib::caba::VciSignals<vci_param> signal_vci_m0("signal_vci_m0");

        //Connet to the ran0
	soclib::caba::VciSignals<vci_param> signal_vci_vcimultiram0("signal_vci_vcimultiram0");

        //To connect VciVgmn and VciMultitty
        //vci_param defined above
	soclib::caba::VciSignals<vci_param> signal_vci_tty("signal_vci_tty");


        ////////////////////////////////////////////////////////////////////
        // Part 4 : instances                                             //
        ////////////////////////////////////////////////////////////////////

        //Initiate the Xcache
	soclib::caba::VciXcacheWrapper<vci_param, soclib::common::Mips32ElIss > cache0("cache0", 0, maptab,IntTab(0), 4,1,8, 4,1,8);
        // Two template parameters :
        //    vci_param                  use for trans
        //    scolib::common::TARGET     iss of the processor to be used
        // 10 parameters :
        //  1 : "cache0"                 name
        //  2 : 0                        vci initiator number
        //  3 : maptab                   reference of the mapping table
        //  4 : IntTab(0)                vci initiator number in IntTab form
        //  5 - 10 :  six characteristics of the cache
        //            instruction cache 4 ways cache 1 set , (4 cache lines) each line contains 8 words  (data cache is the same)

        // ELF loader must be defined before the ram component initiates
	soclib::common::Loader loader("soft/bin.soft");
        // ELF loader object

        // Initiate the Ram0
	soclib::caba::VciRam<vci_param> vcimultiram0("vcimultiram0", IntTab(0), maptab, loader);
        // 1: name
        // 2: vci target id in IntTab
        // 3: maptab
        // 4: ELF bin loader

        // Initiate the TTY
	soclib::caba::VciMultiTty<vci_param> vcitty("vcitty",	IntTab(1), maptab, "vcitty0", NULL);
        // 1: name
        // 2: vci target id in IntTab
        // 3: maptab
        // 4: tty name

        // Initiate the Vgmn
	soclib::caba::VciVgmn<vci_param> vgmn("vgmn",maptab, 1, 2, 2, 8);
        // 1: name
        // 2: maptab
        // 3: number of initiators 1
        // 4: number of targets 2
        // 5-6 : inference delay of fifo is 8

        ////////////////////////////////////////////////////////////////////
        // Part 5 : netlist                                               //
        ////////////////////////////////////////////////////////////////////

	cache0.p_clk(signal_clk);
	cache0.p_resetn(signal_resetn);
	cache0.p_irq[0](signal_mips0_it0);
	cache0.p_irq[1](signal_mips0_it1);
	cache0.p_irq[2](signal_mips0_it2);
	cache0.p_irq[3](signal_mips0_it3);
	cache0.p_irq[4](signal_mips0_it4);
	cache0.p_irq[5](signal_mips0_it5);
	cache0.p_vci(signal_vci_m0);

	vcimultiram0.p_clk(signal_clk);
	vcimultiram0.p_resetn(signal_resetn);
	vcimultiram0.p_vci(signal_vci_vcimultiram0);

	vcitty.p_clk(signal_clk);
	vcitty.p_resetn(signal_resetn);
	vcitty.p_irq[0](signal_tty_irq0);
	vcitty.p_vci(signal_vci_tty);

	vgmn.p_clk(signal_clk);
	vgmn.p_resetn(signal_resetn);
	vgmn.p_to_initiator[0](signal_vci_m0);
	vgmn.p_to_target[0](signal_vci_vcimultiram0);
	vgmn.p_to_target[1](signal_vci_tty);

	sc_start(sc_core::sc_time(0, SC_NS));
	signal_resetn = false;
	sc_start(sc_core::sc_time(1, SC_NS));
	signal_resetn = true;

        ////////////////////////////////////////////////////////////////////
        // Part 6 : simulate                                              //
        ////////////////////////////////////////////////////////////////////

	sc_start();
	return EXIT_SUCCESS;
}

int sc_main (int argc, char *argv[])
{
	try {
		return _main(argc, argv);
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	} catch (...) {
		std::cout << "Unknown exception occured" << std::endl;
		throw;
	}
	return 1;
}
