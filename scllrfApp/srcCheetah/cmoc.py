{ "name" : "cmoc",
     'registers': [ 
    {
    "name" : "Magic", 
        "address": 0x00, 
        "bit_width": 8, 
        "signed": "unsigned",
        "access"      : "r",
        "desc" : "Enter real description here",
        "bits":
        {    "wavesReady" : 0x100 }
    }, 
    {
    "name" : "Dsp_flavor", 
        "address": 0x01, 
        "bit_width": 8, 
        "signed": "unsigned",
        "access"      : "r",
        "desc" : "Enter real description here",
        "bits":
        {    "wavesReady" : 0x100 }
    }, 
    {
    "name" : "Build_year", 
        "address": 0x02, 
        "bit_width": 8, 
        "signed": "unsigned",
        "access"      : "r",
        "desc" : "Year firmware was built",
        "egu"         : "Yr",
        "scale_eqn"   : "x",
        "bits":
        {    "wavesReady" : 0x100 }
    }, 
    {
    "name" : "Build_month", 
        "address": 0x03, 
        "bit_width": 8, 
        "signed": "unsigned",
        "access"      : "r",
        "desc" : "Month firmware was built",
        "egu"         : "Mo",
        "range_top"   : 12,
        "range_bottom": 1,
        "scale_eqn"   : "x",
        "bits":
        {    "wavesReady" : 0x100 }
    }, 
    {
    "name" : "Build_day", 
        "address": 0x04, 
        "bit_width": 8, 
        "signed": "unsigned",
        "access"      : "r",
        "desc" : "Day firmware was built",
        "egu"         : "Dy",
        "range_top"   : 31,
        "range_bottom": 1,
        "scale_eqn"   : "x",
        "bits":
        {    "wavesReady" : 0x100 }
    }, 
    {
    "name" : "Build_hour", 
        "address": 0x05, 
        "bit_width": 8, 
        "signed": "unsigned",
        "access"      : "r",
        "desc" : "Hour firmware was built",
        "egu"         : "Hr",
        "range_top"   : 23,
        "range_bottom": 0,
        "scale_eqn"   : "x",
        "bits":
        {    "wavesReady" : 0x100 }
    }, 
    {
    "name" : "Build_minute", 
        "address": 0x06, 
        "bit_width": 8, 
        "signed": "unsigned",
        "access"      : "r",
        "desc" : "Minute firmware was built",
        "egu"         : "Min",
        "range_top"   : 59,
        "range_bottom": 0,
        "scale_eqn"   : "x",
        "bits":
        {    "wavesReady" : 0x100 }
    }, 
    {
    "name" : "Code_is_clean", 
        "address": 0x07, 
        "bit_width": 8, 
        "signed": "unsigned",
        "access"      : "r",
        "desc" : "Enter real description here",
        "bits":
        {    "wavesReady" : 0x100 }
    }, 
    {
    "name" : "Tool_rev", 
        "address": 0x08, 
        "bit_width": 8, 
        "signed": "unsigned",
        "access"      : "r",
        "desc" : "Enter real description here",
        "bits":
        {    "wavesReady" : 0x100 }
    }, 
    {
    "name" : "User", 
        "address": 0x09, 
        "bit_width": 8, 
        "signed": "unsigned",
        "access"      : "r",
        "desc" : "Name of person compiling firmware",
        "bits":
        {    "wavesReady" : 0x100 }
    }, 
    {
    "name" : "Board_type", 
        "address": 0x0a, 
        "bit_width": 8, 
        "signed": "unsigned",
        "access"      : "r",
        "desc" : "Enter real description here",
        "bits":
        {    "wavesReady" : 0x100 }
    }, 
    {
    "name" : "Version", 
        "address": 0x0b, 
        "bit_width": 8, 
        "signed": "unsigned",
        "access"      : "r",
        "desc" : "Enter real description here",
        "bits":
        {    "wavesReady" : 0x100 }
    }, 
    {
    "name" : "Git_sha1_a", 
        "address": 0x0c, 
        "bit_width": 8, 
        "signed": "unsigned",
        "access"      : "r",
        "desc" : "Enter real description here",
        "bits":
        {    "wavesReady" : 0x100 }
    }, 
    {
    "name" : "Git_sha1_b", 
        "address": 0x0d, 
        "bit_width": 8, 
        "signed": "unsigned",
        "access"      : "r",
        "desc" : "Enter real description here",
        "bits":
        {    "wavesReady" : 0x100 }
    }, 
    {
    "name" : "Git_sha1_c", 
        "address": 0x0e, 
        "bit_width": 8, 
        "signed": "unsigned",
        "access"      : "r",
        "desc" : "Enter real description here",
        "bits":
        {    "wavesReady" : 0x100 }
    }, 
    {
    "name" : "Git_sha1_d", 
        "address": 0x0f, 
        "bit_width": 8, 
        "signed": "unsigned",
        "access"      : "r",
        "desc" : "Enter real description here",
        "bits":
        {    "wavesReady" : 0x100 }
    }, 
    {
    "name" : "Git_sha1_e", 
        "address": 0x10, 
        "bit_width": 8, 
        "signed": "unsigned",
        "access"      : "r",
        "desc" : "Enter real description here",
        "bits":
        {    "wavesReady" : 0x100 }
    }, 
    {
    "name" : "Git_sha1_f", 
        "address": 0x11, 
        "bit_width": 8, 
        "signed": "unsigned",
        "access"      : "r",
        "desc" : "Enter real description here",
        "bits":
        {    "wavesReady" : 0x100 }
    }, 
    {
    "name" : "Git_sha1_g", 
        "address": 0x12, 
        "bit_width": 8, 
        "signed": "unsigned",
        "access"      : "r",
        "desc" : "Enter real description here",
        "bits":
        {    "wavesReady" : 0x100 }
    }, 
    {
    "name" : "Git_sha1_h", 
        "address": 0x13, 
        "bit_width": 8, 
        "signed": "unsigned",
        "access"      : "r",
        "desc" : "Enter real description here",
        "bits":
        {    "wavesReady" : 0x100 }
    }, 
    {
    "name" : "Git_sha1_i", 
        "address": 0x14, 
        "bit_width": 8, 
        "signed": "unsigned",
        "access"      : "r",
        "desc" : "Enter real description here",
        "bits":
        {    "wavesReady" : 0x100 }
    }, 
    {
    "name" : "Git_sha1_j", 
        "address": 0x15, 
        "bit_width": 8, 
        "signed": "unsigned",
        "access"      : "r",
        "desc" : "Enter real description here",
        "bits":
        {    "wavesReady" : 0x100 }
    }, 
    {
    "name" : "Git_sha1_k", 
        "address": 0x16, 
        "bit_width": 8, 
        "signed": "unsigned",
        "access"      : "r",
        "desc" : "Enter real description here",
        "bits":
        {    "wavesReady" : 0x100 }
    }, 
    {
    "name" : "Git_sha1_l", 
        "address": 0x17, 
        "bit_width": 8, 
        "signed": "unsigned",
        "access"      : "r",
        "desc" : "Enter real description here",
        "bits":
        {    "wavesReady" : 0x100 }
    }, 
    {
    "name" : "Git_sha1_m", 
        "address": 0x18, 
        "bit_width": 8, 
        "signed": "unsigned",
        "access"      : "r",
        "desc" : "Enter real description here",
        "bits":
        {    "wavesReady" : 0x100 }
    }, 
    {
    "name" : "Git_sha1_n", 
        "address": 0x19, 
        "bit_width": 8, 
        "signed": "unsigned",
        "access"      : "r",
        "desc" : "Enter real description here",
        "bits":
        {    "wavesReady" : 0x100 }
    }, 
    {
    "name" : "Git_sha1_o", 
        "address": 0x1a, 
        "bit_width": 8, 
        "signed": "unsigned",
        "access"      : "r",
        "desc" : "Enter real description here",
        "bits":
        {    "wavesReady" : 0x100 }
    }, 
    {
    "name" : "Git_sha1_p", 
        "address": 0x1b, 
        "bit_width": 8, 
        "signed": "unsigned",
        "access"      : "r",
        "desc" : "Enter real description here",
        "bits":
        {    "wavesReady" : 0x100 }
    }, 
    {
    "name" : "Git_sha1_q", 
        "address": 0x1c, 
        "bit_width": 8, 
        "signed": "unsigned",
        "access"      : "r",
        "desc" : "Enter real description here",
        "bits":
        {    "wavesReady" : 0x100 }
    }, 
    {
    "name" : "Git_sha1_r", 
        "address": 0x1d, 
        "bit_width": 8, 
        "signed": "unsigned",
        "access"      : "r",
        "desc" : "Enter real description here",
        "bits":
        {    "wavesReady" : 0x100 }
    }, 
    {
    "name" : "Git_sha1_s", 
        "address": 0x1e, 
        "bit_width": 8, 
        "signed": "unsigned",
        "access"      : "r",
        "desc" : "Enter real description here",
        "bits":
        {    "wavesReady" : 0x100 }
    }, 
    {
    "name" : "Git_sha1_t", 
        "address": 0x1f, 
        "bit_width": 8, 
        "signed": "unsigned",
        "access"      : "r",
        "desc" : "Enter real description here",
        "bits":
        {    "wavesReady" : 0x100 }
    },
    {
    "name" :  "dsp_fdbk_core_mp_proc_coeff",
        "nelm": 4, 
        "address": 0, 
        "bit_width": 18, 
        "signed": "signed",
        "access"      : "w",
        "desc" : "Enter real description here"
    }, 
    {
    "name" : "dsp_fdbk_core_mp_proc_lim",
        "nelm": 4, 
        "address": 4, 
        "bit_width": 18, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {
    "name" : "dsp_fdbk_core_mp_proc_setmp",
        "nelm": 4, 
        "address": 8, 
        "bit_width": 18, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {"name" : "dsp_lp_notch_lp1a_kx",
        "nelm": 2, 
        "address": 0xC, 
        "bit_width": 18, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {"name" : "dsp_lp_notch_lp1a_ky",
        "nelm": 2, 
        "address": 0xE, 
        "bit_width": 18, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {
    "name" : "dsp_lp_notch_lp1b_kx",
        "nelm": 2, 
        "address": 0x10, 
        "bit_width": 18, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {
    "name" : "dsp_lp_notch_lp1b_ky",
        "nelm": 2, 
        "address": 0x12, 
        "bit_width": 18, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {
    "name" : "dsp_chan_keep",
        "address": 0x14, 
        "bit_width": 12, 
        "signed": "unsigned",
        "access"      : "w"
    }, 
    {
    "name" : "dsp_fdbk_core_coarse_scale",
        "address": 0x15, 
        "bit_width": 2, 
        "signed": "unsigned",
        "access"      : "w"
    }, 
    {
    "name" : "dsp_fdbk_core_mp_proc_ph_offset",
        "address": 0x16, 
        "bit_width": 18, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {
    "name" : "dsp_fdbk_core_mp_proc_sel_en",
        "address": 0x17, 
        "bit_width": 1, 
        "signed": "unsigned",
        "access"      : "w"
    }, 
    {
    "name" : "dsp_fdbk_core_mp_proc_sel_thresh",
        "address": 0x18, 
        "bit_width": 18, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {
    "name" : "dsp_modulo",
        "address": 0x19, 
        "bit_width": 12, 
        "signed": "unsigned",
        "access"      : "w"
    }, 
    {
    "name" : "dsp_phase_step",
        "address": 0x1A, 
        "bit_width": 32, 
        "signed": "unsigned",
        "access"      : "w"
    }, 
    {
    "name" : "dsp_piezo_piezo_dc",
        "address": 0x1B, 
        "bit_width": 16, 
        "signed": "unsigned",
        "access"      : "w"
    }, 
    {
    "name" : "dsp_tag",
        "address": 0x1C, 
        "bit_width": 8, 
        "signed": "unsigned",
        "access"      : "w"
    }, 
    {
    "name" : "dsp_wave_samp_per",
        "address": 0x1D, 
        "bit_width": 7, 
        "signed": "unsigned",
        "access"      : "w"
    }, 
    {
    "name" : "dsp_wave_shift",
        "address": 0x1E, 
        "bit_width": 3, 
        "signed": "unsigned",
        "access"      : "w"
    }, 
    {
    "name" : "circle_aw",
        "address" : 0x000021,
        "access"  : "r"
    }, 
    {
    "name" : "mode_count",
        "address" : 0x000022,
        "access"  : "r"
    }, 
    {
    "name" : "mode_shift",
        "address" : 0x000023,
        "access"  : "r"
    }, 
    {
    "name" : "n_mech_modes",
    "address" : 0x000024,
        "access"  : "r"
    }, 
    {
    "name" : "df_scale",
        "address" : 0x000025,
        "access"  : "r"
    }, 
    {
    "name" : "simple_demo",
    "address" : 0x000026,
        "access"  : "r"
    },
    {
    "name" : "cav4_mech_noise_cpl_k_out",
        "nelm": 1024, 
        "address": 0x4000,
        "bit_width": 18, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {
    "name" : "cav4_mech_piezo_cpl_k_out",
        "nelm": 1024, 
        "address": 0x4400,
        "bit_width": 18, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {
    "name" : "cav4_mech_resonator_prop_const",
        "nelm": 1024, 
        "address": 0x4800,
        "bit_width": 21, 
        "signed": "unsigned",
        "access"      : "w"
    }, 
    {
    "name" : "sta_cav4_elec_dot_0_k_out",
        "nelm": 1024, 
        "address": 0x4C00,
        "bit_width": 18, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {
    "name" : "sta_cav4_elec_dot_1_k_out",
        "nelm": 1024, 
        "address": 0x5000,
        "bit_width": 18, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {
    "name" : "sta_cav4_elec_dot_2_k_out",
        "nelm": 1024, 
        "address": 0x5400,
        "bit_width": 18, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {
    "name" : "sta_cav4_elec_outer_prod_0_k_out",
        "nelm": 1024, 
        "address": 0x5800, 
        "bit_width": 18, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {
    "name" : "sta_cav4_elec_outer_prod_1_k_out",
        "nelm": 1024, 
        "address": 0x5C00,
        "bit_width": 18, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {
    "name" : "sta_cav4_elec_outer_prod_2_k_out",
        "nelm": 1024, 
        "address": 0x6000, 
        "bit_width": 18, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {
    "name" : "sta_cav4_elec_drv_cpl_out_cplng",
        "addr_width": 1, 
        "address": 0x6400,
        "bit_width": 18, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {
    "name" : "sta_cav4_elec_drv_cpl_out_ph_off",
        "addr_width": 1, 
        "address": 0x6402,
        "bit_width": 19, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {
    "name" : "beam_modulo",
        "addr_width": 0, 
        "address": 0x6410,
        "bit_width": 12, 
        "signed": "unsigned",
        "access"      : "w"
    }, 
    {
    "name" : "beam_phase_step",
        "addr_width": 0, 
        "address": 0x6411,
        "bit_width": 12, 
        "signed": "unsigned",
        "access"      : "w"
    }, 
    {
    "name" : "cav4_mech_prng_iva",
        "addr_width": 0, 
        "address": 0x6412,
        "bit_width": 32, 
        "signed": "unsigned",
        "access"      : "w"
    }, 
    {
    "name" : "cav4_mech_prng_ivb",
        "addr_width": 0, 
        "address": 0x6413,
        "bit_width": 32, 
        "signed": "unsigned",
        "access"      : "w"
    }, 
    {
    "name" : "cav4_mech_prng_random_run",
        "addr_width": 0, 
        "address": 0x6414,
        "bit_width": 1, 
        "signed": "unsigned",
        "access"      : "w"
    }, 
    {
    "name" : "sta_a_cav_offset",
        "addr_width": 0, 
        "address": 0x6415,
        "bit_width": 10, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {
    "name" : "sta_a_for_offset",
        "addr_width": 0, 
        "address": 0x6416,
        "bit_width": 10, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {
    "name" : "sta_a_rfl_offset",
        "addr_width": 0, 
        "address": 0x6417,
        "bit_width": 10, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {
    "name" : "sta_amp_lp_bw",
        "addr_width": 0, 
        "address": 0x6418,
        "bit_width": 18, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {
    "name" : "sta_cav4_elec_freq_0_coarse_freq",
        "addr_width": 0, 
        "address": 0x6419,
        "bit_width": 28, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {
    "name" : "sta_cav4_elec_freq_1_coarse_freq",
        "addr_width": 0, 
        "address": 0x641A,
        "bit_width": 28, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {
    "name" : "sta_cav4_elec_freq_2_coarse_freq",
        "addr_width": 0, 
        "address": 0x641B,
        "bit_width": 28, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {
    "name" : "sta_cav4_elec_mode_0_beam_cpl",
        "addr_width": 0, 
        "address": 0x641C, 
        "bit_width": 18, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {
    "name" : "sta_cav4_elec_mode_0_bw",
        "addr_width": 0, 
        "address": 0x641D,
        "bit_width": 18, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {
    "name" : "sta_cav4_elec_mode_0_drv_cpl",
        "addr_width": 0, 
        "address": 0x641E,
        "bit_width": 18, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {
    "name" : "sta_cav4_elec_mode_0_out_cpl_out_cplng",
        "nelm": 2, 
        "address": 0x6404, 
        "bit_width": 18, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {
    "name" : "sta_cav4_elec_mode_0_out_cpl_out_ph_off",
        "addr_width": 1, 
        "address": 0x6406, 
        "bit_width": 19, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {
    "name" : "sta_cav4_elec_mode_1_beam_cpl",
        "addr_width": 0, 
        "address": 0x641F, 
        "bit_width": 18, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {
    "name" : "sta_cav4_elec_mode_1_bw",
        "addr_width": 0, 
        "address": 0x6420, 
        "bit_width": 18, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {
    "name" : "sta_cav4_elec_mode_1_drv_cpl",
        "addr_width": 0, 
        "address": 0x6421, 
        "bit_width": 18, 
        "signed": "signed",
        "access"      : "w"
    },  
    {
    "name" : "sta_cav4_elec_mode_2_beam_cpl",
        "addr_width": 0, 
        "address": 0x6422, 
        "bit_width": 18, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {
    "name" : "sta_cav4_elec_mode_2_bw",
        "addr_width": 0, 
        "address": 0x6423, 
        "bit_width": 18, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {
    "name" : "sta_cav4_elec_mode_2_drv_cpl",
        "addr_width": 0, 
        "address": 0x6424, 
        "bit_width": 18, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {
    "name" : "sta_cav4_elec_modulo",
        "addr_width": 0, 
        "address": 0x6425, 
        "bit_width": 12, 
        "signed": "unsigned",
        "access"      : "w"
    }, 
    {
    "name" : "sta_cav4_elec_phase_step",
        "addr_width": 0, 
        "address": 0x6426, 
        "bit_width": 32, 
        "signed": "unsigned",
        "access"      : "w"
    }, 
    {
    "name" : "sta_compr_sat_ctl",
        "addr_width": 0, 
        "address": 0x6427, 
        "bit_width": 16, 
        "signed": "unsigned",
        "access"      : "w"
    }, 
    {
    "name" : "sta_prng_iva",
        "addr_width": 0, 
        "address": 0x6428, 
        "bit_width": 32, 
        "signed": "unsigned",
        "access"      : "w"
    }, 
    {
    "name" : "sta_prng_ivb",
        "addr_width": 0, 
        "address": 0x6429, 
        "bit_width": 32, 
        "signed": "unsigned",
        "access"      : "w"
    }, 
    {
    "name" : "sta_prng_random_run",
        "addr_width": 0, 
        "address": 0x642a, 
        "bit_width": 1, 
        "signed": "unsigned"
    },
    {
    "name" : "sta_cav4_elec_mode_1_out_cpl_out_cplng",
        "addr_width": 1, 
        "address": 0x642b, 
        "bit_width": 18, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {
    "name" : "sta_cav4_elec_mode_1_out_cpl_out_ph_off",
        "addr_width": 1, 
        "address": 0x640A, 
        "bit_width": 19, 
        "signed": "signed"
    },
    {
    "name" : "sta_cav4_elec_mode_2_out_cpl_out_cplng",
        "addr_width": 1, 
        "address": 0x640C, 
        "bit_width": 18, 
        "signed": "signed",
        "access"      : "w"
    }, 
    {
    "name" : "sta_cav4_elec_mode_2_out_cpl_out_ph_off",
        "addr_width": 1, 
        "address": 0x640E, 
        "bit_width": 19, 
        "signed": "signed",
        "access"      : "w"
    }, 
                    ]
}