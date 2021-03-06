-- Cameron L Palmer
-- ELET 2720 Section 305
-- Lab 4 - Standard SOP Design

LIBRARY ieee;
USE ieee.std_logic_1164.all;

--------------------------------------------------

entity ADVISOR_ent is
port(
	X: out bit; Y: out bit;
	A: in bit; B: in bit; C: in bit; D: in bit; E: in bit
);
end ADVISOR_ent;

--------------------------------------------------

architecture ADVISOR_arc of ADVISOR_ent is
begin
X <= (A and B and C and (not D) and (not E)) or (A and B and C and (not D) and E) or (A and B and C and D and (not E)) or (A and B and C and D and E);
Y <= ((not A) and (not B) and C and D and E) or ((not A) and B and C and D and E) or (A and (not B) and C and D and E) or (A and B and C and D and E);
end ADVISOR_arc;
