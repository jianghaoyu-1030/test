`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/07/11 21:21:27
// Design Name: 
// Module Name: bram
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


module bram
(
    input  FCLK_CLK0,
    input  aresetn,

    output BRAM_PORTB_en  ,
    output [3:0]BRAM_PORTB_we  ,
    output BRAM_PORTB_rst ,
    output BRAM_PORTB_clk ,
    output [31:0]BRAM_PORTB_addr,
    output [31:0]BRAM_PORTB_din ,
    input  [31:0]BRAM_PORTB_dout,

    input GPIO_tri_o_0
);

   reg gpio_tri_o_0_reg; 
   reg ps_bram_wr_done;
   reg pl_bram_wr_done; 
   reg bram_en; 
   reg [3:0]  bram_we; 
   reg [31:0] bram_addr; 
   reg [31:0] bram_rd_data; 
   reg [31:0] bram_wr_data; 
   reg [2:0] bram_state; 

   reg [31:0] read_data_num;
   reg [31:0] rec_data_bram[63:0];
   reg bram_rec_over;
//////////////////////////////////////////////////////////////////////////    
//    localparam  BRAM_ADDRESS_HIGH = 32'd4096 - 32'd4; 
    
   always@(posedge FCLK_CLK0) 
     begin 
         if(!aresetn) 
             gpio_tri_o_0_reg <= 1'b0; 
         else 
             gpio_tri_o_0_reg <= GPIO_tri_o_0; 
     end 
    
   always@(posedge FCLK_CLK0) 
     begin 
         if(!aresetn) 
             ps_bram_wr_done <= 1'b0; 
         else if({gpio_tri_o_0_reg, GPIO_tri_o_0} == 2'b01) //gpio0 rising edge 
             ps_bram_wr_done <= 1'b1; 
         else 
             ps_bram_wr_done <= 1'b0; 
     end 
///////////////////////////////////////////////////////////////////////////////
always@(posedge FCLK_CLK0) 
     begin 
         if(!aresetn) begin 
             bram_we <= 4'd0; 
             bram_en <= 1'b0; 
             bram_addr <= 32'd0; 
             bram_state <= 3'd0; 
         end 
         else begin 
             case(bram_state) 
                        0:begin
                            bram_state <= 1;         //改变读写
                            bram_en <= 1'b0;
                            bram_we <= 4'd0;
                            bram_addr <= 32'd0;
                            //   rx_tx_cmd_flag <= 1'd0;
                         end
                       1: begin                    //数据线翻转接收PS端指令
                            if(ps_bram_wr_done)    //接收到ps端数据
                               begin
                                   bram_en <= 1'b0;
                                   bram_we <= 4'd0;
                                   bram_state <= 2;
                                   bram_rec_over<= 0;
                                   bram_addr <= 0;     
                                   read_data_num <= 0;           
                               end
                            else   bram_state <= 1;
                          end 
                       2: begin
                            bram_en <= 1'b1;
                            bram_we <= 4'd0;
                            bram_state <= 3; 
                          end
                        3: begin
                                rec_data_bram[read_data_num] = BRAM_PORTB_dout;
                                read_data_num <=  read_data_num + 1'd1;  
                                bram_addr <= bram_addr + 32'd4;
                                 if( read_data_num < 32'd10)
                                 begin
                                      bram_state <= 3;
                                      bram_rec_over<= 1;                                     
                                 end
                                 else
                                 begin                                                                          
                                      bram_en <= 1'b0;
                                      bram_we <= 4'd0;
                                      bram_addr <= 32'd0;
                                      read_data_num<=  0;
                                      bram_state <= 0;
                                      //rx_tx_cmd_flag <= 1'd1;
                                 end
                        end                  
                     default: bram_state <= 0;
             endcase 
         end 
     end 
      
   assign BRAM_PORTB_en   = bram_en; 
   assign BRAM_PORTB_we   = bram_we; 
   assign BRAM_PORTB_rst  = ~aresetn; 
   assign BRAM_PORTB_clk  = FCLK_CLK0; 
   assign BRAM_PORTB_addr = bram_addr; 
//    assign BRAM_PORTB_din  = bram_wr_data; 
//    assign GPIO_tri_i_1 = pl_bram_wr_done; 

ila_0 ila_0_i (
	.clk(FCLK_CLK0), // input wire clk


	.probe0(BRAM_PORTB_en), // 1  
	.probe1(BRAM_PORTB_rst), // 1
	.probe2(BRAM_PORTB_addr), // 32 
	.probe3(BRAM_PORTB_dout), // 32 
	.probe4(BRAM_PORTB_we), // 4 
	.probe5(ps_bram_wr_done), // 1 
	.probe6(read_data_num), // 32 
	.probe7(bram_rec_over), // 1
    .probe8(bram_state)//3
);

endmodule
