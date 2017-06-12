d = {
    "name": "scllrfRES", 
    "registers": [
        {
            "access": "rw", 
            "address": 0, 
            "description": "acceleration Hz/(Sec^2)", 
            "name": "motor1_acc", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 1, 
            "description": "velocity Hz/Sec", 
            "name": "motor1_vlcty", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 2, 
            "description": "steps Hz", 
            "name": "motor1_steps", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 3, 
            "description": "signed steps Hz", 
            "name": "motor1_sgn_steps", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "address": 4, 
            "description": "absolute steps Hz", 
            "name": "motor1_abs_steps", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 5, 
            "description": "last acceleration Hz/(Sec^2)", 
            "name": "motor1_lacc", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 6, 
            "description": "last velocity Hz/Sec", 
            "name": "motor1_lvlcty", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 7, 
            "description": "last steps Hz", 
            "name": "motor1_lsteps", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 8, 
            "description": "drive current Amps", 
            "name": "motor1_drvI", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 9, 
            "description": "board temp deg C", 
            "name": "motor1_brd_tmp", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 10, 
            "bits": {
                "abs_sub_inhibit": 11, 
                "auto_mode": 7, 
                "clear_done_flag": 4, 
                "clr_abs_steps": 9, 
                "clr_sgn_steps": 8, 
                "crash_motor": 2, 
                "direction": 5, 
                "go": 0, 
                "inhbit_mask": 1, 
                "low_current_mask": 6, 
                "sgn_sub_inhibit": 10, 
                "slow_gdr_enable": 12, 
                "stop_motor": 3
            }, 
            "description": "control bits", 
            "name": "motor1_cntl", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 11, 
            "bits": {
                "abs_step_limit": 9, 
                "any_limit": 5, 
                "deta_disc": 11, 
                "inhbit_mask": 6, 
                "last_direction": 1, 
                "limit_high": 4, 
                "limit_low": 3, 
                "low_current": 7, 
                "motor_done": 2, 
                "motor_moving": 0, 
                "sign_step_limit": 8, 
                "slow_gdr": 12, 
                "step_inhibit": 10
            }, 
            "description": "status bits", 
            "name": "motor1_stat", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 12, 
            "description": "drive dac1 set point Volts", 
            "name": "piezo1_dac1", 
            "signed": "signed"
        }, 
        {
            "access": "rw", 
            "address": 13, 
            "description": "drive dac2 set point Volts", 
            "name": "piezo1_dac2", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "address": 14, 
            "description": "position readback adc1 Volts", 
            "name": "piezo1_adc1", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "address": 15, 
            "description": "position readback adc2 Volts", 
            "name": "piezo1_adc2", 
            "signed": "signed"
        }, 
        {
            "access": "rw", 
            "address": 16, 
            "description": "position limit high Volts", 
            "name": "piezo1_high_window", 
            "signed": "signed"
        }, 
        {
            "access": "rw", 
            "address": 17, 
            "description": "position limit low Volts", 
            "name": "piezo1_low_window", 
            "signed": "signed"
        }, 
        {
            "access": "rw", 
            "address": 18, 
            "bits": {
                "piezo_adc_reset": 11, 
                "piezo_board_enable": 0, 
                "piezo_buff_trig_0": 3, 
                "piezo_buff_trig_1": 4, 
                "piezo_buff_trig_2": 5, 
                "piezo_buff_trig_3": 6, 
                "piezo_buff_trig_4": 7, 
                "piezo_buff_trig_5": 8, 
                "piezo_buff_trig_6": 9, 
                "piezo_buff_trig_7": 10, 
                "piezo_capture_enable": 1, 
                "piezo_dac_enable": 2, 
                "piezo_debug0": 12, 
                "piezo_debug1": 13, 
                "piezo_dual_dac_update": 17
            }, 
            "description": "control bits", 
            "name": "piezo1_cntl", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 19, 
            "bits": {
                "adc_busy": 1, 
                "dac_busy": 0
            }, 
            "description": "status bits", 
            "name": "piezo1_stat", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 20, 
            "description": "board temp deg C", 
            "name": "piezo1_brd_tmp", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 21, 
            "description": "serial number", 
            "name": "piezo1_brd_sernum", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 22, 
            "description": "I controller gain", 
            "name": "piezo1_igain", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 23, 
            "description": "detune frequency error Hz", 
            "name": "cavity1_detune_err", 
            "signed": "signed"
        }, 
        {
            "access": "rw", 
            "address": 24, 
            "description": "acceleration Hz/(Sec^2)", 
            "name": "motor2_acc", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 25, 
            "description": "velocity Hz/Sec", 
            "name": "motor2_vlcty", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 26, 
            "description": "steps Hz", 
            "name": "motor2_steps", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 27, 
            "description": "signed steps Hz", 
            "name": "motor2_sgn_steps", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "address": 28, 
            "description": "absolute steps Hz", 
            "name": "motor2_abs_steps", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 29, 
            "description": "last acceleration Hz/(Sec^2)", 
            "name": "motor2_lacc", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 30, 
            "description": "last velocity Hz/Sec", 
            "name": "motor2_lvlcty", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 31, 
            "description": "last steps Hz", 
            "name": "motor2_lsteps", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 32, 
            "description": "motor drive current Amps", 
            "name": "motor2_drvI", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 33, 
            "description": "board temp deg C", 
            "name": "motor2_brd_tmp", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 34, 
            "bits": {
                "abs_sub_inhibit": 11, 
                "auto_mode": 7, 
                "clear_done_flag": 4, 
                "clr_abs_steps": 9, 
                "clr_sgn_steps": 8, 
                "crash_motor": 2, 
                "direction": 5, 
                "go": 0, 
                "inhbit_mask": 1, 
                "low_current_mask": 6, 
                "sgn_sub_inhibit": 10, 
                "slow_gdr_enable": 12, 
                "stop_motor": 3
            }, 
            "description": "control bits", 
            "name": "motor2_cntl", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 35, 
            "bits": {
                "abs_step_limit": 9, 
                "any_limit": 5, 
                "deta_disc": 11, 
                "inhbit_mask": 6, 
                "last_direction": 1, 
                "limit_high": 4, 
                "limit_low": 3, 
                "low_current": 7, 
                "motor_done": 2, 
                "motor_moving": 0, 
                "sign_step_limit": 8, 
                "slow_gdr": 12, 
                "step_inhibit": 10
            }, 
            "description": "status bits", 
            "name": "motor2_stat", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 36, 
            "description": "drive dac1 set point Volts", 
            "name": "piezo2_dac1", 
            "signed": "signed"
        }, 
        {
            "access": "rw", 
            "address": 37, 
            "description": "drive dac2 set point Volts", 
            "name": "piezo2_dac2", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "address": 38, 
            "description": "position readback adc1 Volts", 
            "name": "piezo2_adc1", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "address": 39, 
            "description": "position readback adc2 Volts", 
            "name": "piezo2_adc2", 
            "signed": "signed"
        }, 
        {
            "access": "rw", 
            "address": 40, 
            "description": "position limit high Volts", 
            "name": "piezo2_high_window", 
            "signed": "signed"
        }, 
        {
            "access": "rw", 
            "address": 41, 
            "description": "position limit low Volts", 
            "name": "piezo2_low_window", 
            "signed": "signed"
        }, 
        {
            "access": "rw", 
            "address": 42, 
            "bits": {
                "piezo_adc_reset": 11, 
                "piezo_board_enable": 0, 
                "piezo_buff_trig_0": 3, 
                "piezo_buff_trig_1": 4, 
                "piezo_buff_trig_2": 5, 
                "piezo_buff_trig_3": 6, 
                "piezo_buff_trig_4": 7, 
                "piezo_buff_trig_5": 8, 
                "piezo_buff_trig_6": 9, 
                "piezo_buff_trig_7": 10, 
                "piezo_capture_enable": 1, 
                "piezo_dac_enable": 2, 
                "piezo_debug0": 12, 
                "piezo_debug1": 13, 
                "piezo_dual_dac_update": 17
            }, 
            "description": "control bits", 
            "name": "piezo2_cntl", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 43, 
            "bits": {
                "adc_busy": 1, 
                "dac_busy": 0
            }, 
            "description": "status bits", 
            "name": "piezo2_stat", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 44, 
            "description": "board temp deg C", 
            "name": "piezo2_brd_tmp", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 45, 
            "description": "serial number", 
            "name": "piezo2_brd_sernum", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 46, 
            "description": "I controller gain", 
            "name": "piezo2_igain", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 47, 
            "description": "detune frequency error Hz", 
            "name": "cavity2_detune_err", 
            "signed": "signed"
        }, 
        {
            "access": "rw", 
            "address": 48, 
            "description": "acceleration Hz/(Sec^2)", 
            "name": "motor3_acc", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 49, 
            "description": "velocity Hz/Sec", 
            "name": "motor3_vlcty", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 50, 
            "description": "steps Hz", 
            "name": "motor3_steps", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 51, 
            "description": "signed steps Hz", 
            "name": "motor3_sgn_steps", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "address": 52, 
            "description": "absolute steps Hz", 
            "name": "motor3_abs_steps", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 53, 
            "description": "last acceleration Hz/(Sec^2)", 
            "name": "motor3_lacc", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 54, 
            "description": "last velocity Hz/Sec", 
            "name": "motor3_lvlcty", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 55, 
            "description": "last steps Hz", 
            "name": "motor3_lsteps", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 56, 
            "description": "drive current Amps", 
            "name": "motor3_drvI", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 57, 
            "description": "board temp deg C", 
            "name": "motor3_brd_tmp", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 58, 
            "bits": {
                "abs_sub_inhibit": 11, 
                "auto_mode": 7, 
                "clear_done_flag": 4, 
                "clr_abs_steps": 9, 
                "clr_sgn_steps": 8, 
                "crash_motor": 2, 
                "direction": 5, 
                "go": 0, 
                "inhbit_mask": 1, 
                "low_current_mask": 6, 
                "sgn_sub_inhibit": 10, 
                "slow_gdr_enable": 12, 
                "stop_motor": 3
            }, 
            "description": "control bits", 
            "name": "motor3_cntl", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 59, 
            "bits": {
                "abs_step_limit": 9, 
                "any_limit": 5, 
                "deta_disc": 11, 
                "inhbit_mask": 6, 
                "last_direction": 1, 
                "limit_high": 4, 
                "limit_low": 3, 
                "low_current": 7, 
                "motor_done": 2, 
                "motor_moving": 0, 
                "sign_step_limit": 8, 
                "slow_gdr": 12, 
                "step_inhibit": 10
            }, 
            "description": "status bits", 
            "name": "motor3_stat", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 60, 
            "description": "drive dac1 set point Volts", 
            "name": "piezo3_dac1", 
            "signed": "signed"
        }, 
        {
            "access": "rw", 
            "address": 61, 
            "description": "drive dac2 set point Volts", 
            "name": "piezo3_dac2", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "address": 62, 
            "description": "position readback adc1 Volts", 
            "name": "piezo3_adc1", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "address": 63, 
            "description": "position readback adc2 Volts", 
            "name": "piezo3_adc2", 
            "signed": "signed"
        }, 
        {
            "access": "rw", 
            "address": 64, 
            "description": "position limit high Volts", 
            "name": "piezo3_high_window", 
            "signed": "signed"
        }, 
        {
            "access": "rw", 
            "address": 65, 
            "description": "position limit low Volts", 
            "name": "piezo3_low_window", 
            "signed": "signed"
        }, 
        {
            "access": "rw", 
            "address": 66, 
            "bits": {
                "piezo_adc_reset": 11, 
                "piezo_board_enable": 0, 
                "piezo_buff_trig_0": 3, 
                "piezo_buff_trig_1": 4, 
                "piezo_buff_trig_2": 5, 
                "piezo_buff_trig_3": 6, 
                "piezo_buff_trig_4": 7, 
                "piezo_buff_trig_5": 8, 
                "piezo_buff_trig_6": 9, 
                "piezo_buff_trig_7": 10, 
                "piezo_capture_enable": 1, 
                "piezo_dac_enable": 2, 
                "piezo_debug0": 12, 
                "piezo_debug1": 13, 
                "piezo_dual_dac_update": 17
            }, 
            "description": "control bits", 
            "name": "piezo3_cntl", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 67, 
            "bits": {
                "adc_busy": 1, 
                "dac_busy": 0
            }, 
            "description": "status bits", 
            "name": "piezo3_stat", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 68, 
            "description": "board temp deg C", 
            "name": "piezo3_brd_tmp", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 69, 
            "description": "serial number", 
            "name": "piezo3_brd_sernum", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 70, 
            "description": "I controller gain", 
            "name": "piezo3_igain", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 71, 
            "description": "detune frequency error Hz", 
            "name": "cavity3_detune_err", 
            "signed": "signed"
        }, 
        {
            "access": "rw", 
            "address": 72, 
            "description": "acceleration Hz/(Sec^2)", 
            "name": "motor4_acc", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 73, 
            "description": "velocity Hz/Sec", 
            "name": "motor4_vlcty", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 74, 
            "description": "steps Hz", 
            "name": "motor4_steps", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 75, 
            "description": "signed steps Hz", 
            "name": "motor4_sgn_steps", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "address": 76, 
            "description": "absolute steps Hz", 
            "name": "motor4_abs_steps", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 77, 
            "description": "last acceleration Hz/(Sec^2)", 
            "name": "motor4_lacc", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 78, 
            "description": "last velocity Hz/Sec", 
            "name": "motor4_lvlcty", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 79, 
            "description": "last steps Hz", 
            "name": "motor4_lsteps", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 80, 
            "description": "drive current Amps", 
            "name": "motor4_drvI", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 81, 
            "description": "board temp deg C", 
            "name": "motor4_brd_tmp", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 82, 
            "bits": {
                "abs_sub_inhibit": 11, 
                "auto_mode": 7, 
                "clear_done_flag": 4, 
                "clr_abs_steps": 9, 
                "clr_sgn_steps": 8, 
                "crash_motor": 2, 
                "direction": 5, 
                "go": 0, 
                "inhbit_mask": 1, 
                "low_current_mask": 6, 
                "sgn_sub_inhibit": 10, 
                "slow_gdr_enable": 12, 
                "stop_motor": 3
            }, 
            "description": "control bits", 
            "name": "motor4_cntl", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 83, 
            "bits": {
                "abs_step_limit": 9, 
                "any_limit": 5, 
                "deta_disc": 11, 
                "inhbit_mask": 6, 
                "last_direction": 1, 
                "limit_high": 4, 
                "limit_low": 3, 
                "low_current": 7, 
                "motor_done": 2, 
                "motor_moving": 0, 
                "sign_step_limit": 8, 
                "slow_gdr": 12, 
                "step_inhibit": 10
            }, 
            "description": "status bits", 
            "name": "motor4_stat", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 84, 
            "description": "drive dac1 set point Volts", 
            "name": "piezo4_dac1", 
            "signed": "signed"
        }, 
        {
            "access": "rw", 
            "address": 85, 
            "description": "drive dac2 set point Volts", 
            "name": "piezo4_dac2", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "address": 86, 
            "description": "position readback adc1 Volts", 
            "name": "piezo4_adc1", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "address": 87, 
            "description": "position readback adc2 Volts", 
            "name": "piezo4_adc2", 
            "signed": "signed"
        }, 
        {
            "access": "rw", 
            "address": 88, 
            "description": "position limit high Volts", 
            "name": "piezo4_high_window", 
            "signed": "signed"
        }, 
        {
            "access": "rw", 
            "address": 89, 
            "description": "position limit low Volts", 
            "name": "piezo4_low_window", 
            "signed": "signed"
        }, 
        {
            "access": "rw", 
            "address": 90, 
            "bits": {
                "piezo_adc_reset": 11, 
                "piezo_board_enable": 0, 
                "piezo_buff_trig_0": 3, 
                "piezo_buff_trig_1": 4, 
                "piezo_buff_trig_2": 5, 
                "piezo_buff_trig_3": 6, 
                "piezo_buff_trig_4": 7, 
                "piezo_buff_trig_5": 8, 
                "piezo_buff_trig_6": 9, 
                "piezo_buff_trig_7": 10, 
                "piezo_capture_enable": 1, 
                "piezo_dac_enable": 2, 
                "piezo_debug0": 12, 
                "piezo_debug1": 13, 
                "piezo_dual_dac_update": 17
            }, 
            "description": "control bits", 
            "name": "piezo4_cntl", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 91, 
            "bits": {
                "adc_busy": 1, 
                "dac_busy": 0
            }, 
            "description": "status bits", 
            "name": "piezo4_stat", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 92, 
            "description": "board temp deg C", 
            "name": "piezo4_brd_tmp", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 93, 
            "description": "serial number", 
            "name": "piezo4_brd_sernum", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 94, 
            "description": "I controller gain", 
            "name": "piezo4_igain", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 95, 
            "description": "detune frequency error Hz", 
            "name": "cavity4_detune_err", 
            "signed": "signed"
        }, 
        {
            "access": "rw", 
            "address": 96, 
            "description": "Steps to subtract from step registers", 
            "name": "motor1_sub_stp", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 97, 
            "description": "Steps to subtract from step registers", 
            "name": "motor2_sub_stp", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 98, 
            "description": "Steps to subtract from step registers", 
            "name": "motor3_sub_stp", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 99, 
            "description": "Steps to subtract from step registers", 
            "name": "motor4_sub_stp", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 100, 
            "description": "", 
            "name": "reserved0", 
            "signed": ""
        }, 
        {
            "access": "rw", 
            "address": 101, 
            "description": "", 
            "name": "reserved1", 
            "signed": ""
        }, 
        {
            "access": "rw", 
            "address": 102, 
            "description": "", 
            "name": "reserved2", 
            "signed": ""
        }, 
        {
            "access": "rw", 
            "address": 103, 
            "description": "", 
            "name": "reserved3", 
            "signed": ""
        }, 
        {
            "access": "rw", 
            "address": 104, 
            "description": "", 
            "name": "reserved4", 
            "signed": ""
        }, 
        {
            "access": "r", 
            "address": 105, 
            "description": "Actual steps moved", 
            "name": "motor1_steps_actual", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 106, 
            "description": "Actual steps moved", 
            "name": "motor2_steps_actual", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 107, 
            "description": "Actual steps moved", 
            "name": "motor3_steps_actual", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 108, 
            "description": "Actual steps moved", 
            "name": "motor4_steps_actual", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 111, 
            "description": "PZT waveform availability: 8 bits per piezo, MSB, pzt 4-1", 
            "name": "pzt_wave_avail", 
            "signed": "unsigned",
            "bits": {}
        }, 
        {
            "access": "r", 
            "nelm": "1024", 
            "address": 112, 
            "description": "Piezo 1 chan 1 current n", 
            "name": "piezo1_c1_In", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "nelm": "1024", 
            "address": 113, 
            "description": "Piezo 1 chan 1 current p", 
            "name": "piezo1_c1_Ip", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "nelm": "1024", 
            "address": 114, 
            "description": "Piezo 1 chan 1 out voltage scaled", 
            "name": "piezo1_c1_out_v", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "nelm": "1024", 
            "address": 115, 
            "description": "Piezo 1 chan 1 amp drive voltage", 
            "name": "piezo1_c1_drive_v", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "nelm": "1024", 
            "address": 116, 
            "description": "Piezo 1 chan 2 current n", 
            "name": "piezo1_c2_In", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "nelm": "1024", 
            "address": 117, 
            "description": "Piezo 1 chan 2 current p", 
            "name": "piezo1_c2_Ip", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "nelm": "1024", 
            "address": 118, 
            "description": "Piezo 1 chan 2 out voltage scaled", 
            "name": "piezo1_c2_out_v", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "nelm": "1024", 
            "address": 119, 
            "description": "Piezo 1 chan 2 amp drive voltage", 
            "name": "piezo1_c2_drive_v", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "nelm": "1024", 
            "address": 128, 
            "description": "Piezo 2 chan 1 current n", 
            "name": "piezo2_c1_In", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "nelm": "1024", 
            "address": 129, 
            "description": "Piezo 2 chan 1 current p", 
            "name": "piezo2_c1_Ip", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "nelm": "1024", 
            "address": 130, 
            "description": "Piezo 2 chan 1 out voltage scaled", 
            "name": "piezo2_c1_out_v", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "nelm": "1024", 
            "address": 131, 
            "description": "Piezo 2 chan 1 amp drive voltage", 
            "name": "piezo2_c1_drive_v", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "nelm": "1024", 
            "address": 132, 
            "description": "Piezo 2 chan 2 current n", 
            "name": "piezo2_c2_In", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "nelm": "1024", 
            "address": 133, 
            "description": "Piezo 2 chan 2 current p", 
            "name": "piezo2_c2_Ip", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "nelm": "1024", 
            "address": 134, 
            "description": "Piezo 2 chan 2 out voltage scaled", 
            "name": "piezo2_c2_out_v", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "nelm": "1024", 
            "address": 135, 
            "description": "Piezo 2 chan 2 amp drive voltage", 
            "name": "piezo2_c2_drive_v", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "nelm": "1024", 
            "address": 144, 
            "description": "Piezo 3 chan 1 current n", 
            "name": "piezo3_c1_In", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "nelm": "1024", 
            "address": 145, 
            "description": "Piezo 3 chan 1 current p", 
            "name": "piezo3_c1_Ip", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "nelm": "1024", 
            "address": 146, 
            "description": "Piezo 3 chan 1 out voltage scaled", 
            "name": "piezo3_c1_out_v", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "nelm": "1024", 
            "address": 147, 
            "description": "Piezo 3 chan 1 amp drive voltage", 
            "name": "piezo3_c1_drive_v", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "nelm": "1024", 
            "address": 148, 
            "description": "Piezo 3 chan 2 current n", 
            "name": "piezo3_c2_In", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "nelm": "1024", 
            "address": 149, 
            "description": "Piezo 3 chan 2 current p", 
            "name": "piezo3_c2_Ip", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "nelm": "1024", 
            "address": 150, 
            "description": "Piezo 3 chan 2 out voltage scaled", 
            "name": "piezo3_c2_out_v", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "nelm": "1024", 
            "address": 151, 
            "description": "Piezo 3 chan 2 amp drive voltage", 
            "name": "piezo3_c2_drive_v", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "nelm": "1024", 
            "address": 160, 
            "description": "Piezo 4 chan 1 current n", 
            "name": "piezo4_c1_In", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "nelm": "1024", 
            "address": 161, 
            "description": "Piezo 4 chan 1 current p", 
            "name": "piezo4_c1_Ip", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "nelm": "1024", 
            "address": 162, 
            "description": "Piezo 4 chan 1 out voltage scaled", 
            "name": "piezo4_c1_out_v", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "nelm": "1024", 
            "address": 163, 
            "description": "Piezo 4 chan 1 amp drive voltage", 
            "name": "piezo4_c1_drive_v", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "nelm": "1024", 
            "address": 164, 
            "description": "Piezo 4 chan 2 current n", 
            "name": "piezo4_c2_In", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "nelm": "1024", 
            "address": 165, 
            "description": "Piezo 4 chan 2 current p", 
            "name": "piezo4_c2_Ip", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "nelm": "1024", 
            "address": 166, 
            "description": "Piezo 4 chan 2 out voltage scaled", 
            "name": "piezo4_c2_out_v", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "nelm": "1024", 
            "address": 167, 
            "description": "Piezo 4 chan 2 amp drive voltage", 
            "name": "piezo4_c2_drive_v", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "address": 176, 
            "description": "", 
            "name": "motor1_period", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 177, 
            "description": "", 
            "name": "motor2_period", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 178, 
            "description": "", 
            "name": "motor3_period", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 179, 
            "description": "", 
            "name": "motor4_period", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 180, 
            "description": "", 
            "name": "cct0_cav0_status", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 181, 
            "description": "", 
            "name": "cct0_cav1_status", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 182, 
            "description": "", 
            "name": "cct1_cav0_status", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 183, 
            "description": "", 
            "name": "cct1_cav1_status", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 184, 
            "description": "", 
            "name": "cc0_freq_gtx_tx", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 185, 
            "description": "", 
            "name": "cc0_freq_gtx_rx", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 186, 
            "description": "", 
            "name": "cc1_freq_gtx_tx", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 187, 
            "description": "", 
            "name": "cc1_freq_gtx_rx", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 192, 
            "description": "", 
            "name": "cav1_status", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 193, 
            "description": "", 
            "name": "cav2_status", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 194, 
            "description": "", 
            "name": "cav3_status", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 195, 
            "description": "", 
            "name": "cav4_status", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 196, 
            "bits": {
                "ccr_error": 14, 
                "ccr_faults0": 10, 
                "ccr_faults1": 11, 
                "ccr_faults2": 12, 
                "ccr_faults3": 13, 
                "ccr_gateware_type0": 3, 
                "ccr_gateware_type1": 4, 
                "ccr_gateware_type2": 5, 
                "ccr_location0": 0, 
                "ccr_location1": 1, 
                "ccr_location2": 2, 
                "ccr_protocol_ver0": 6, 
                "ccr_protocol_ver1": 7, 
                "ccr_protocol_ver2": 8, 
                "ccr_protocol_ver3": 9
            }, 
            "description": "", 
            "name": "ccr_summary", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 198, 
            "description": "", 
            "name": "ccr0_revid", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 199, 
            "description": "", 
            "name": "ccr1_revid", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 200, 
            "description": "", 
            "name": "cc0_latency", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 201, 
            "description": "", 
            "name": "cc1_latency", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 202, 
            "description": "", 
            "name": "ccr0_counters", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 203, 
            "description": "", 
            "name": "ccr1_counters", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 204, 
            "description": "", 
            "name": "cc_control", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 208, 
            "description": "", 
            "name": "qsfp_i2c_reg", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 209, 
            "description": "", 
            "name": "p_input_src", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 210, 
            "bits": {
                "U30_reset": 13, 
                "U50_reset": 12
            }, 
            "description": "", 
            "name": "periph_config", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 211, 
            "bits": {
                "U32_soft_reset_0": 4, 
                "U32_soft_reset_1": 5, 
                "U32_soft_reset_2": 6, 
                "U32_soft_reset_3": 7, 
                "U50_soft_reset_0": 0, 
                "U50_soft_reset_1": 1, 
                "U50_soft_reset_2": 2, 
                "U50_soft_reset_3": 3
            }, 
            "description": "", 
            "name": "fiber_reset", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 212, 
            "description": "", 
            "name": "sfp_address_set", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 213, 
            "description": "", 
            "name": "ccr0_ctrace_start", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 214, 
            "description": "", 
            "name": "ccr0_ctrace_status", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 215, 
            "description": "", 
            "name": "ccr1_ctrace_start", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 216, 
            "description": "", 
            "name": "ccr1_ctrace_status", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 217, 
            "description": "", 
            "name": "cct0_ctrace_start", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 218, 
            "description": "", 
            "name": "cct0_ctrace_status", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 219, 
            "description": "", 
            "name": "cct1_ctrace_start", 
            "signed": "unsigned"
        }, 
        {
            "access": "r", 
            "address": 220, 
            "description": "", 
            "name": "cct1_ctrace_status", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 223, 
            "description": "", 
            "name": "ctlr_int_control", 
            "signed": "unsigned"
        }, 
        {
            "access": "rw", 
            "address": 224, 
            "description": "", 
            "name": "dac1_sp", 
            "signed": "signed"
        }, 
        {
            "access": "rw", 
            "address": 225, 
            "description": "", 
            "name": "dac2_sp", 
            "signed": "signed"
        }, 
        {
            "access": "rw", 
            "address": 226, 
            "description": "", 
            "name": "dac3_sp", 
            "signed": "signed"
        }, 
        {
            "access": "rw", 
            "address": 227, 
            "description": "", 
            "name": "dac4_sp", 
            "signed": "signed"
        }, 
        {
            "access": "rw", 
            "address": 228, 
            "description": "", 
            "name": "dac1_min", 
            "signed": "signed"
        }, 
        {
            "access": "rw", 
            "address": 229, 
            "description": "", 
            "name": "dac2_min", 
            "signed": "signed"
        }, 
        {
            "access": "rw", 
            "address": 230, 
            "description": "", 
            "name": "dac3_min", 
            "signed": "signed"
        }, 
        {
            "access": "rw", 
            "address": 231, 
            "description": "", 
            "name": "dac4_min", 
            "signed": "signed"
        }, 
        {
            "access": "rw", 
            "address": 232, 
            "description": "", 
            "name": "dac1_max", 
            "signed": "signed"
        }, 
        {
            "access": "rw", 
            "address": 233, 
            "description": "", 
            "name": "dac2_max", 
            "signed": "signed"
        }, 
        {
            "access": "rw", 
            "address": 234, 
            "description": "", 
            "name": "dac3_max", 
            "signed": "signed"
        }, 
        {
            "access": "rw", 
            "address": 235, 
            "description": "", 
            "name": "dac4_max", 
            "signed": "signed"
        }, 
        {
            "access": "rw", 
            "address": 236, 
            "description": "", 
            "name": "ctlr1_out", 
            "signed": "signed"
        }, 
        {
            "access": "rw", 
            "address": 237, 
            "description": "", 
            "name": "ctlr2_out", 
            "signed": "signed"
        }, 
        {
            "access": "rw", 
            "address": 238, 
            "description": "", 
            "name": "ctlr3_out", 
            "signed": "signed"
        }, 
        {
            "access": "rw", 
            "address": 239, 
            "description": "", 
            "name": "ctlr4_out", 
            "signed": "signed"
        }, 
        {
            "access": "r", 
            "address": 240, 
            "description": "", 
            "name": "usr_access", 
            "signed": "unsigned"
        }
    ]
}