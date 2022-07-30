`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/07/06 16:55:59
// Design Name: 
// Module Name: top
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module top
(
  inout [14:0]DDR_addr,
  inout [2:0]DDR_ba,
  inout DDR_cas_n,
  inout DDR_ck_n,
  inout DDR_ck_p,
  inout DDR_cke,
  inout DDR_cs_n,
  inout [3:0]DDR_dm,
  inout [31:0]DDR_dq,
  inout [3:0]DDR_dqs_n,
  inout [3:0]DDR_dqs_p,
  inout DDR_odt,
  inout DDR_ras_n,
  inout DDR_reset_n,
  inout DDR_we_n,

  inout FIXED_IO_ddr_vrn,
  inout FIXED_IO_ddr_vrp,
  inout [53:0]FIXED_IO_mio,
  inout FIXED_IO_ps_clk,
  inout FIXED_IO_ps_porb,
  inout FIXED_IO_ps_srstb
);


//wire define
wire FCLK_CLK1;
wire peripheral_aresetn;
wire [31:0] S_AXIS_tdata ;
wire [3:0]  S_AXIS_tkeep ;
wire S_AXIS_tlast ;
wire S_AXIS_tready;
wire S_AXIS_tvalid;
wire gpio_dma_tri_o;
wire s_axis_aresetn;
wire s_axis_aclk;
/////bram////
wire [31:0]BRAM_PORTB_addr;
wire BRAM_PORTB_clk;
wire [31:0]BRAM_PORTB_din;
wire [31:0]BRAM_PORTB_dout;
wire BRAM_PORTB_en;
wire BRAM_PORTB_rst;
wire [3:0]BRAM_PORTB_we;
wire gpio_bram_tri_o;



//////////////////instantiate////////////////////
system_wrapper u_system_wrapper(
  .BRAM_PORTB_addr    (BRAM_PORTB_addr    ),
  .BRAM_PORTB_clk     (BRAM_PORTB_clk     ),
  .BRAM_PORTB_din     (BRAM_PORTB_din     ),
  .BRAM_PORTB_dout    (BRAM_PORTB_dout    ),
  .BRAM_PORTB_en      (BRAM_PORTB_en      ),
  .BRAM_PORTB_rst     (BRAM_PORTB_rst     ),
  .BRAM_PORTB_we      (BRAM_PORTB_we      ),

  .DDR_addr           (DDR_addr           ),
  .DDR_ba             (DDR_ba             ),
  .DDR_cas_n          (DDR_cas_n          ),
  .DDR_ck_n           (DDR_ck_n           ),
  .DDR_ck_p           (DDR_ck_p           ),
  .DDR_cke            (DDR_cke            ),
  .DDR_cs_n           (DDR_cs_n           ),
  .DDR_dm             (DDR_dm             ),
  .DDR_dq             (DDR_dq             ),
  .DDR_dqs_n          (DDR_dqs_n          ),
  .DDR_dqs_p          (DDR_dqs_p          ),
  .DDR_odt            (DDR_odt            ),
  .DDR_ras_n          (DDR_ras_n          ),
  .DDR_reset_n        (DDR_reset_n        ),
  .DDR_we_n           (DDR_we_n           ),

  .FIXED_IO_ddr_vrn   (FIXED_IO_ddr_vrn   ),
  .FIXED_IO_ddr_vrp   (FIXED_IO_ddr_vrp   ),
  .FIXED_IO_mio       (FIXED_IO_mio       ),
  .FIXED_IO_ps_clk    (FIXED_IO_ps_clk    ),
  .FIXED_IO_ps_porb   (FIXED_IO_ps_porb   ),
  .FIXED_IO_ps_srstb  (FIXED_IO_ps_srstb  ),

  .S_AXIS_tdata       (S_AXIS_tdata       ),
  .S_AXIS_tkeep       (S_AXIS_tkeep       ),
  .S_AXIS_tlast       (S_AXIS_tlast       ),
  .S_AXIS_tready      (S_AXIS_tready      ),
  .S_AXIS_tvalid      (S_AXIS_tvalid      ),

  .gpio_bram_tri_o    (gpio_bram_tri_o    ),
  .gpio_dma_tri_o     (gpio_dma_tri_o     ),

  .FCLK_CLK1          (FCLK_CLK1          ),
  .peripheral_aresetn (peripheral_aresetn ),

  .s_axis_aclk        (s_axis_aclk        ),
  .s_axis_aresetn     (s_axis_aresetn     )
);


assign s_axis_aclk = FCLK_CLK1;
assign s_axis_aresetn = peripheral_aresetn;

data_test u_data_test(
    .clk           (FCLK_CLK1),
    .rst_n         (peripheral_aresetn),
    .S_AXIS_tready (S_AXIS_tready ),
    .gpio_tri_o_0  (gpio_dma_tri_o ),
    .S_AXIS_tvalid (S_AXIS_tvalid ),
    .S_AXIS_tlast  (S_AXIS_tlast  ),
    .S_AXIS_tkeep  (S_AXIS_tkeep  ),
    .S_AXIS_tdata  (S_AXIS_tdata  )
);

bram u_bram(
  .FCLK_CLK0       (FCLK_CLK1       ),
  .aresetn         (peripheral_aresetn),
  .BRAM_PORTB_en   (BRAM_PORTB_en   ),
  .BRAM_PORTB_we   (BRAM_PORTB_we   ),
  .BRAM_PORTB_rst  (BRAM_PORTB_rst  ),
  .BRAM_PORTB_clk  (BRAM_PORTB_clk  ),
  .BRAM_PORTB_addr (BRAM_PORTB_addr ),
  .BRAM_PORTB_din  (BRAM_PORTB_din  ),
  .BRAM_PORTB_dout (BRAM_PORTB_dout ),
  .GPIO_tri_o_0    (gpio_bram_tri_o )
);




endmodule
