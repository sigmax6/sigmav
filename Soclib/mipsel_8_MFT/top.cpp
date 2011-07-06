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
 * Maintainers: nipo
 */

#include <iostream>
#include <cstdlib>

#include "mapping_table.h"
#include "mips.h"
#include "ississ2.h"
#include "vci_xcache_wrapper.h"
#include "vci_timer.h"
#include "vci_ram.h"
#include "vci_multi_tty.h"
#include "vci_vgmn.h"
#include "vci_fd_access.h"

#include "segmentation.h"

int _main(int argc, char *argv[])
{
	using namespace sc_core;
	// Avoid repeating these everywhere
	using soclib::common::IntTab;
	using soclib::common::Segment;

	// Define our VCI parameters
	typedef soclib::caba::VciParams<4,9,32,1,1,1,8,1,1,1> vci_param;

	// Mapping table

	soclib::common::MappingTable maptab(32, IntTab(8), IntTab(8), 0x00300000);

	maptab.add(Segment("reset", RESET_BASE, RESET_SIZE, IntTab(1), true));
	maptab.add(Segment("excep", EXCEP_BASE, EXCEP_SIZE, IntTab(1), true));
	maptab.add(Segment("text" , TEXT_BASE , TEXT_SIZE , IntTab(1), true));
	maptab.add(Segment("data" , DATA_BASE , DATA_SIZE , IntTab(1), true));

	maptab.add(Segment("timer", TIMER_BASE, TIMER_SIZE, IntTab(3), false));
	maptab.add(Segment("tty"  , TTY_BASE  , TTY_SIZE  , IntTab(2), false));
    maptab.add(Segment("fd"   , FD_BASE   , FD_SIZE   , IntTab(0), false));

	// Signals

	sc_clock		signal_clk("signal_clk");
	sc_signal<bool> signal_resetn("signal_resetn");

	sc_signal<bool> signal_mips0_it0("signal_mips0_it0");
	sc_signal<bool> signal_mips0_it1("signal_mips0_it1");
	sc_signal<bool> signal_mips0_it2("signal_mips0_it2");
	sc_signal<bool> signal_mips0_it3("signal_mips0_it3");
	sc_signal<bool> signal_mips0_it4("signal_mips0_it4");
	sc_signal<bool> signal_mips0_it5("signal_mips0_it5");

	sc_signal<bool> signal_mips1_it0("signal_mips1_it0");
	sc_signal<bool> signal_mips1_it1("signal_mips1_it1");
	sc_signal<bool> signal_mips1_it2("signal_mips1_it2");
	sc_signal<bool> signal_mips1_it3("signal_mips1_it3");
	sc_signal<bool> signal_mips1_it4("signal_mips1_it4");
	sc_signal<bool> signal_mips1_it5("signal_mips1_it5");

	sc_signal<bool> signal_mips2_it0("signal_mips2_it0");
	sc_signal<bool> signal_mips2_it1("signal_mips2_it1");
	sc_signal<bool> signal_mips2_it2("signal_mips2_it2");
	sc_signal<bool> signal_mips2_it3("signal_mips2_it3");
	sc_signal<bool> signal_mips2_it4("signal_mips2_it4");
	sc_signal<bool> signal_mips2_it5("signal_mips2_it5");

	sc_signal<bool> signal_mips3_it0("signal_mips3_it0");
	sc_signal<bool> signal_mips3_it1("signal_mips3_it1");
	sc_signal<bool> signal_mips3_it2("signal_mips3_it2");
	sc_signal<bool> signal_mips3_it3("signal_mips3_it3");
	sc_signal<bool> signal_mips3_it4("signal_mips3_it4");
	sc_signal<bool> signal_mips3_it5("signal_mips3_it5");

	sc_signal<bool> signal_mips4_it0("signal_mips4_it0");
	sc_signal<bool> signal_mips4_it1("signal_mips4_it1");
	sc_signal<bool> signal_mips4_it2("signal_mips4_it2");
	sc_signal<bool> signal_mips4_it3("signal_mips4_it3");
	sc_signal<bool> signal_mips4_it4("signal_mips4_it4");
	sc_signal<bool> signal_mips4_it5("signal_mips4_it5");

	sc_signal<bool> signal_mips5_it0("signal_mips5_it0");
	sc_signal<bool> signal_mips5_it1("signal_mips5_it1");
	sc_signal<bool> signal_mips5_it2("signal_mips5_it2");
	sc_signal<bool> signal_mips5_it3("signal_mips5_it3");
	sc_signal<bool> signal_mips5_it4("signal_mips5_it4");
	sc_signal<bool> signal_mips5_it5("signal_mips5_it5");

	sc_signal<bool> signal_mips6_it0("signal_mips6_it0");
	sc_signal<bool> signal_mips6_it1("signal_mips6_it1");
	sc_signal<bool> signal_mips6_it2("signal_mips6_it2");
	sc_signal<bool> signal_mips6_it3("signal_mips6_it3");
	sc_signal<bool> signal_mips6_it4("signal_mips6_it4");
	sc_signal<bool> signal_mips6_it5("signal_mips6_it5");

	sc_signal<bool> signal_mips7_it0("signal_mips7_it0");
	sc_signal<bool> signal_mips7_it1("signal_mips7_it1");
	sc_signal<bool> signal_mips7_it2("signal_mips7_it2");
	sc_signal<bool> signal_mips7_it3("signal_mips7_it3");
	sc_signal<bool> signal_mips7_it4("signal_mips7_it4");
	sc_signal<bool> signal_mips7_it5("signal_mips7_it5");

	soclib::caba::VciSignals<vci_param> signal_vci_m0("signal_vci_m0");
	soclib::caba::VciSignals<vci_param> signal_vci_m1("signal_vci_m1");
	soclib::caba::VciSignals<vci_param> signal_vci_m2("signal_vci_m2");
	soclib::caba::VciSignals<vci_param> signal_vci_m3("signal_vci_m3");
	soclib::caba::VciSignals<vci_param> signal_vci_m4("signal_vci_m4");
	soclib::caba::VciSignals<vci_param> signal_vci_m5("signal_vci_m5");
	soclib::caba::VciSignals<vci_param> signal_vci_m6("signal_vci_m6");
	soclib::caba::VciSignals<vci_param> signal_vci_m7("signal_vci_m7");

	soclib::caba::VciSignals<vci_param> signal_vci_tty("signal_vci_tty");
	soclib::caba::VciSignals<vci_param> signal_vci_vcimultiram0("signal_vci_vcimultiram0");
	soclib::caba::VciSignals<vci_param> signal_vci_vcitimer("signal_vci_vcitimer");
    soclib::caba::VciSignals<vci_param> signal_vci_vcifd("signal_vci_vcifd");
    soclib::caba::VciSignals<vci_param> signal_vci_vcifd_i("signal_vci_vcifd_i");



	sc_signal<bool> signal_tty_irq0("signal_tty_irq0");
	sc_signal<bool> signal_tty_irq1("signal_tty_irq1");
	sc_signal<bool> signal_tty_irq2("signal_tty_irq2");
	sc_signal<bool> signal_tty_irq3("signal_tty_irq3");
	sc_signal<bool> signal_tty_irq4("signal_tty_irq4");
	sc_signal<bool> signal_tty_irq5("signal_tty_irq5");
	sc_signal<bool> signal_tty_irq6("signal_tty_irq6");
	sc_signal<bool> signal_tty_irq7("signal_tty_irq7");

    sc_signal<bool> signal_timer_irq0("signal_timer_irq0");

	// Components

	soclib::caba::VciXcacheWrapper<vci_param, soclib::common::IssIss2<soclib::common::MipsElIss> > cache0("cache0", 0, maptab,IntTab(1), 4,1,8, 4,1,8);
	soclib::caba::VciXcacheWrapper<vci_param, soclib::common::IssIss2<soclib::common::MipsElIss> > cache1("cache1", 1, maptab,IntTab(2), 4,1,8, 4,1,8);
	soclib::caba::VciXcacheWrapper<vci_param, soclib::common::IssIss2<soclib::common::MipsElIss> > cache2("cache2", 2, maptab,IntTab(3), 4,1,8, 4,1,8);
	soclib::caba::VciXcacheWrapper<vci_param, soclib::common::IssIss2<soclib::common::MipsElIss> > cache3("cache3", 3, maptab,IntTab(4), 4,1,8, 4,1,8);
	soclib::caba::VciXcacheWrapper<vci_param, soclib::common::IssIss2<soclib::common::MipsElIss> > cache4("cache4", 4, maptab,IntTab(5), 4,1,8, 4,1,8);
	soclib::caba::VciXcacheWrapper<vci_param, soclib::common::IssIss2<soclib::common::MipsElIss> > cache5("cache5", 5, maptab,IntTab(6), 4,1,8, 4,1,8);
	soclib::caba::VciXcacheWrapper<vci_param, soclib::common::IssIss2<soclib::common::MipsElIss> > cache6("cache6", 6, maptab,IntTab(7), 4,1,8, 4,1,8);
	soclib::caba::VciXcacheWrapper<vci_param, soclib::common::IssIss2<soclib::common::MipsElIss> > cache7("cache7", 7, maptab,IntTab(8), 4,1,8, 4,1,8);
/*
	soclib::caba::VciXcacheWrapper<vci_param, soclib::common::Mips32ElIss > cache1("cache1", 1, maptab,IntTab(1), 4,1,8, 4,1,8);
	soclib::caba::VciXcacheWrapper<vci_param, soclib::common::Mips32ElIss > cache2("cache2", 2, maptab,IntTab(2), 4,1,8, 4,1,8);
	soclib::caba::VciXcacheWrapper<vci_param, soclib::common::Mips32ElIss > cache3("cache3", 3, maptab,IntTab(3), 4,1,8, 4,1,8);
    */

	soclib::common::Loader loader("soft/bin.soft");
	soclib::caba::VciRam<vci_param> vcimultiram0("vcimultiram0", IntTab(1), maptab, loader);
	soclib::caba::VciMultiTty<vci_param> vcitty("vcitty",	IntTab(2), maptab, "vcitty0", "vcitty1", "vcitty2", "vcitty3","vcitty4","vcitty5","vcitty6","vcitty7", NULL);
   // soclib::caba::VciMultiTty<vci_param> vcitty("vcitty",	IntTab(2), maptab, "vcitty0",NULL);// "vcitty1", "vcitty2", "vcitty3", NULL);
	soclib::caba::VciTimer<vci_param> vcitimer("vcitimer", IntTab(3), maptab, 8);
    soclib::caba::VciFdAccess<vci_param> vcifd("vcifd"   , maptab , IntTab(0), IntTab(0)); //srcid 0 tgtid 0
	soclib::caba::VciVgmn<vci_param> vgmn("vgmn",maptab, 9, 4, 2, 8);

	//	Net-List

	cache0.p_clk(signal_clk);
	cache1.p_clk(signal_clk);
	cache2.p_clk(signal_clk);
	cache3.p_clk(signal_clk);
	cache4.p_clk(signal_clk);
	cache5.p_clk(signal_clk);
	cache6.p_clk(signal_clk);
	cache7.p_clk(signal_clk);
	vcimultiram0.p_clk(signal_clk);
	vcitimer.p_clk(signal_clk);

	cache0.p_resetn(signal_resetn);
	cache1.p_resetn(signal_resetn);
	cache2.p_resetn(signal_resetn);
	cache3.p_resetn(signal_resetn);
	cache4.p_resetn(signal_resetn);
	cache5.p_resetn(signal_resetn);
	cache6.p_resetn(signal_resetn);
	cache7.p_resetn(signal_resetn);

	vcimultiram0.p_resetn(signal_resetn);
	vcitimer.p_resetn(signal_resetn);

	cache0.p_irq[0](signal_mips0_it0);
	cache0.p_irq[1](signal_mips0_it1);
	cache0.p_irq[2](signal_mips0_it2);
	cache0.p_irq[3](signal_mips0_it3);
	cache0.p_irq[4](signal_mips0_it4);
	cache0.p_irq[5](signal_mips0_it5);

	cache1.p_irq[0](signal_mips1_it0);
	cache1.p_irq[1](signal_mips1_it1);
	cache1.p_irq[2](signal_mips1_it2);
	cache1.p_irq[3](signal_mips1_it3);
	cache1.p_irq[4](signal_mips1_it4);
	cache1.p_irq[5](signal_mips1_it5);

	cache2.p_irq[0](signal_mips2_it0);
	cache2.p_irq[1](signal_mips2_it1);
	cache2.p_irq[2](signal_mips2_it2);
	cache2.p_irq[3](signal_mips2_it3);
	cache2.p_irq[4](signal_mips2_it4);
	cache2.p_irq[5](signal_mips2_it5);

	cache3.p_irq[0](signal_mips3_it0);
	cache3.p_irq[1](signal_mips3_it1);
	cache3.p_irq[2](signal_mips3_it2);
	cache3.p_irq[3](signal_mips3_it3);
	cache3.p_irq[4](signal_mips3_it4);
	cache3.p_irq[5](signal_mips3_it5);

	cache4.p_irq[0](signal_mips4_it0);
	cache4.p_irq[1](signal_mips4_it1);
	cache4.p_irq[2](signal_mips4_it2);
	cache4.p_irq[3](signal_mips4_it3);
	cache4.p_irq[4](signal_mips4_it4);
	cache4.p_irq[5](signal_mips4_it5);

	cache5.p_irq[0](signal_mips5_it0);
	cache5.p_irq[1](signal_mips5_it1);
	cache5.p_irq[2](signal_mips5_it2);
	cache5.p_irq[3](signal_mips5_it3);
	cache5.p_irq[4](signal_mips5_it4);
	cache5.p_irq[5](signal_mips5_it5);

	cache6.p_irq[0](signal_mips6_it0);
	cache6.p_irq[1](signal_mips6_it1);
	cache6.p_irq[2](signal_mips6_it2);
	cache6.p_irq[3](signal_mips6_it3);
	cache6.p_irq[4](signal_mips6_it4);
	cache6.p_irq[5](signal_mips6_it5);

	cache7.p_irq[0](signal_mips7_it0);
	cache7.p_irq[1](signal_mips7_it1);
	cache7.p_irq[2](signal_mips7_it2);
	cache7.p_irq[3](signal_mips7_it3);
	cache7.p_irq[4](signal_mips7_it4);
	cache7.p_irq[5](signal_mips7_it5);


	cache0.p_vci(signal_vci_m0);
	cache1.p_vci(signal_vci_m1);
	cache2.p_vci(signal_vci_m2);
	cache3.p_vci(signal_vci_m3);
	cache4.p_vci(signal_vci_m4);
	cache5.p_vci(signal_vci_m5);
    cache6.p_vci(signal_vci_m6);
    cache7.p_vci(signal_vci_m7);

	vcimultiram0.p_vci(signal_vci_vcimultiram0);

	vcitimer.p_vci(signal_vci_vcitimer);
	vcitimer.p_irq[0](signal_timer_irq0);
	vcitimer.p_irq[1](signal_mips1_it0);
	vcitimer.p_irq[2](signal_mips2_it0);
	vcitimer.p_irq[3](signal_mips3_it0);
	vcitimer.p_irq[4](signal_mips4_it0);
	vcitimer.p_irq[5](signal_mips5_it0);
	vcitimer.p_irq[6](signal_mips6_it0);
	vcitimer.p_irq[7](signal_mips7_it0);

	vcitty.p_clk(signal_clk);
	vcitty.p_resetn(signal_resetn);
	vcitty.p_vci(signal_vci_tty);
	vcitty.p_irq[0](signal_tty_irq0);
	vcitty.p_irq[1](signal_tty_irq1);
	vcitty.p_irq[2](signal_tty_irq2);
	vcitty.p_irq[3](signal_tty_irq3);
	vcitty.p_irq[4](signal_tty_irq4);
	vcitty.p_irq[5](signal_tty_irq5);
	vcitty.p_irq[6](signal_tty_irq6);
	vcitty.p_irq[7](signal_tty_irq7);


    vcifd.p_clk(signal_clk);
    vcifd.p_resetn(signal_resetn);
    vcifd.p_irq(signal_mips0_it0);
    vcifd.p_vci_target(signal_vci_vcifd);
    vcifd.p_vci_initiator(signal_vci_vcifd_i);

	vgmn.p_clk(signal_clk);
	vgmn.p_resetn(signal_resetn);

	vgmn.p_to_initiator[1](signal_vci_m0);
    vgmn.p_to_initiator[0](signal_vci_vcifd_i);
	vgmn.p_to_initiator[2](signal_vci_m1);
	vgmn.p_to_initiator[3](signal_vci_m2);
	vgmn.p_to_initiator[4](signal_vci_m3);
	vgmn.p_to_initiator[5](signal_vci_m4);
	vgmn.p_to_initiator[6](signal_vci_m5);
	vgmn.p_to_initiator[7](signal_vci_m6);
	vgmn.p_to_initiator[8](signal_vci_m7);

	vgmn.p_to_target[1](signal_vci_vcimultiram0);
	vgmn.p_to_target[3](signal_vci_vcitimer);
	vgmn.p_to_target[2](signal_vci_tty);
    vgmn.p_to_target[0](signal_vci_vcifd);

	sc_start(sc_core::sc_time(0, SC_NS));
	signal_resetn = false;
	sc_start(sc_core::sc_time(1, SC_NS));
	signal_resetn = true;

#ifdef SOCVIEW
	debug();
#else
	sc_start();
#endif
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
