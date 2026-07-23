# 4x4x4-RGB-LED-Cube

4x4x4 multiplexed RGB LED cube featuring custom CAD designed manufacturing jigs, embedded C firmware, and shift register control.

I first want to get into how I created the jig to solder the LEDs.

<img width="2237" height="1208" alt="Jig Model" src="https://github.com/user-attachments/assets/3757588d-2eae-4618-9b58-01a73f7ffcba" />

**Things to take into consideration while creating your jig**

I wanted the gap between each led to be 2.5cm since it will provide more space to work with and the visibility is better.

The leg of the led will be soldered along the edge of the wood so by having a smaller distance from the led to the edge would have the wires be closer together making the cube look neat.

My leds are Diffused RGB LEDs common cathode and they have a diameter of 0.5cm. 

When I created my laser cut files I chose a diameter slighly smaller since laser cutters have some tolerance to how precise they can cut along the circle.

I went with 0.49cm which worked great for me, any of the ones which were slightly tighter I just sanded down the inside until the LED fit snug.

As for the cuts on the board you should adjust the width of the slot to be slightly smaller than the thickness of your plywood (my plywood was 0.4cm).

It is much easier to sand down a part of the plywood down till it fits into the slot.

In my case I had to sand down 0.05cm of the plywood for it to fit into the slot till it was snug.

As you glue down the LED holders into the sockets check that they're lined up so that the LED columns are symmetrical.

**Troubles I ran into while creating this**

As you may have noticed I created this design in Autodesk Inventor, but the laser file needed to be in an SVG file format for the laser cutter I used. 

To get around this I originally saved it as a DXF file then used an online converter to get it into a SVG. 

I then uploaded the SVG file to Inkscape to make sure the dimensions were correct, after verifying the dimensions I set the stroke width to 0.025mm and the color to red.

<img width="1856" height="1363" alt="Cainkpture" src="https://github.com/user-attachments/assets/93f3f528-df11-4c88-86f7-bc1605bfd452" />

Now the file is ready for printing. 

I cannot give much insight into the values for the power and speed of your laser cutter since they vary between different laser cutters.

If you are interested I used the "Trotec Speedy 400 - 120 watt C02 Laser" at 100% power and 0.3 speed since my plywood was on the thicker side.

**Assembly**

<img width="4000" height="3000" alt="20260723_161809" src="https://github.com/user-attachments/assets/b2e49303-73e9-49c0-8bd4-2d776d1d70fc" />

The pieces you have should look similar to these.

<img width="4000" height="3000" alt="20260723_161832" src="https://github.com/user-attachments/assets/47b0b40e-258c-4694-b3e3-9eea8302ce3b" />

As you can see the LED Jig is thicker than the slot it should go in which is why I sanded/cut it until it was 0.35cm.

<img width="3000" height="4000" alt="20260723_171716" src="https://github.com/user-attachments/assets/19efe971-fbe7-4182-8c5a-cf5f3b357aa4" />

Now that you have all 5 pieces like this you can begin to assemble the jig.

Use super glue or wood glue and make sure they're properly aligned so that the LEDs stay straight.

<img width="4000" height="3000" alt="20260723_161915" src="https://github.com/user-attachments/assets/da548024-6d75-418f-afa3-d1257ff03603" />

The fully assembled jig should look similar to this.

<img width="3000" height="4000" alt="20260723_162244" src="https://github.com/user-attachments/assets/6acbe197-9f61-4fa5-b296-2ea704df4208" />

<img width="3000" height="1352" alt="20260723_162320" src="https://github.com/user-attachments/assets/1a147e04-70ab-4144-8a05-10fd5d0a199f" />

Now that we have this we need to get into the more tedious part of this project.

But first you may be asking why I used 5 LED slots if this cube is 4x4x4. 

The reason behind this is because I originally planned to use the last one to clip the wire in place during soldering, but so far I cannot say how useful it will be.

