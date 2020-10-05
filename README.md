# NotBots-SPICE
<p>
This is the repo for our SPICE project. It uses the Eigen library to do linear algebra and MATLAB to plot output waves.
</p>
<p> 
The NotBots-SPICE simulator takes SPICE netlist files as input and produces text files as output. The text files can then be used to produce easy-to-read plots using the included MATLAB script.
</p> 
<p><b> Supported components </b></p>
<ul>
  <li> Resistors </li>
  <li> DC voltage and current sources </li>
  <li> SINE voltage and current sources </li>
  <li> Capacitors </li>
  <li> Inductors </li>
  <li> Diodes </li>
</ul>
<p><b> Supported types of analyses </b></p>
<ul>
  <li>Operating Point Analysis <b>(.op)</b></li>
  <li>Transient Analysis <b>(.tran)</b></li>
</ul>
<p><b> How to simulate </b></p>
<ol>
  <li>Compile the 'simulate.cpp' source file using C++11 or later.</li>
  <li>Run the executable and feed it your netlist file as input (examples in the .sh script files).</li>
  <li>Observe that the 'data_out.txt' file now contains the output of the simulation in text format.</li>
  <li>Open and run the 'plotsim.m' MATLAB script.</li>
  <li>Enjoy the colorful plots!</li>
</ol>
<p><b>Extra features</b></p>
<ul>
  <li>The simulator can take arbitrary node names in the netlist and preserve them in the output file.</li>
  <li>Simulating voltage sources in parallel will throw an error.</li>
  <li>Simulating parallel inductors works by automatically adding small series resistances in series with each inductor.</li>
</ul>
<p>
Created by: Youngmin Song and Mihai-Catalin Stretcu<br/>
Department of Electrical and Electronic Engineering, Imperial College London
</p>
