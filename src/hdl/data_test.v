`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2022/06/08 11:12:05
// Design Name: 
// Module Name: data_test
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


module data_test
(
    input clk,
    input rst_n,


    input       S_AXIS_tready,
    input [0:0] gpio_tri_o_0,

    output        S_AXIS_tvalid,
    output        S_AXIS_tlast,
    output [3:0]  S_AXIS_tkeep,
    output [31:0] S_AXIS_tdata

);

reg [1:0]  state;
reg        AXIS_tvalid;
reg [31:0] AXIS_tdata;
reg        AXIS_tlast;

assign S_AXIS_tkeep = 4'b1111;
assign S_AXIS_tvalid = AXIS_tvalid;
assign S_AXIS_tdata = AXIS_tdata;
assign S_AXIS_tlast = AXIS_tlast;

always@(posedge clk)
 begin
     if(!rst_n) begin
         AXIS_tvalid <= 1'b0;
         AXIS_tdata <= 32'd0;
         AXIS_tlast <= 1'b0;
         state <=0;
     end
     else begin
        case(state)
          0: begin
              if(gpio_tri_o_0&& S_AXIS_tready) begin
                 AXIS_tvalid <= 1'b1;
                 state <= 1;
              end
              else begin
                 AXIS_tvalid <= 1'b0;
                 state <= 0;
              end
            end
          1:begin
               if(S_AXIS_tready) begin
                   AXIS_tdata <= AXIS_tdata + 1'b1;
                   if(S_AXIS_tdata == 16'd510) begin
                      AXIS_tlast <= 1'b1;
                      state <= 2;
                   end
                   else begin
                      AXIS_tlast <= 1'b0;
                      state <= 1;
                   end
               end
               else begin
                  AXIS_tdata <= AXIS_tdata;                   
                  state <= 1;
               end
            end       
          2:begin
               if(!S_AXIS_tready) begin
                  AXIS_tvalid <= 1'b1;
                  AXIS_tlast <= 1'b1;
                  AXIS_tdata <= AXIS_tdata;
                  state <= 2;
               end
               else begin
                  AXIS_tvalid <= 1'b0;
                  AXIS_tlast <= 1'b0;
                  AXIS_tdata <= 32'd0;
                  state <= 0;
               end
            end
         default: state <=0;
         endcase
     end              
 end



endmodule
