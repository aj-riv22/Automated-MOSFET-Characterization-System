port = "COM3"; % Select the Arduino's serial port.
arduino = serialport(port, 115200, "Timeout", 10); % Open the port at 115200 baud; wait at most 10 seconds when reading.
configureTerminator(arduino, "LF"); % Treat a line feed as the end of each incoming line.

appliedVoltage = zeros(1, 51); % Preallocate space for 51 applied-voltage values.
sensedVoltage = zeros(1, 51); % Preallocate space for 51 measured-voltage values.
sampleCount = 0; % Count how many complete voltage pairs have been received.
fprintf("Reading voltage samples from %s...\n", port); % Announce that serial capture has started.

while sampleCount < 51 % Keep reading until 51 valid pairs have been received.
	line = strtrim(readline(arduino)); % Read one complete serial line and remove whitespace at its ends.
	values = str2double(split(line, ",")); % Split a comma-separated line and convert its values to numbers.

	if numel(values) == 2 && all(isfinite(values)) % Accept only lines containing exactly two valid numbers.
		sampleCount = sampleCount + 1; % Advance to the next sample position.
		appliedVoltage(sampleCount) = values(1); % Store the first CSV value as the applied voltage.
		sensedVoltage(sampleCount) = values(2); % Store the second CSV value as the sensed voltage.
		fprintf("Sample %02d: Applied = %.3f V, Sensed = %.3f V\n", sampleCount, appliedVoltage(sampleCount), sensedVoltage(sampleCount)); % Print this pair in MATLAB's Command Window.
	end % Ignore startup and cycle-status lines that are not numeric pairs.
end % Finish collecting after both arrays contain 51 values.

clear arduino; % Close the serial connection; keep the voltage arrays in the workspace.
fprintf("Capture complete: %d voltage pairs received.\n", sampleCount); % Report that all samples were collected.

figure; % Open a new plot window.
plot(appliedVoltage, sensedVoltage, "-o", "LineWidth", 1.5); % Plot sensed voltage against applied voltage.
grid on; % Show a grid to make voltage values easier to read.
xlabel("Applied Voltage (V)"); % Label the horizontal axis.
ylabel("Sensed Voltage (V)"); % Label the vertical axis.
title("Sensed Voltage vs. Applied Voltage"); % Describe the plotted relationship.
axis tight; % Adjust the axes limits to fit the data tightly.

% only works in saturation region because sqrt(id) = sqrt(kn/2) * (Vgs - Vth) => y = mx + b
% sqrt(sensed) vs applied voltage
figure; % Open a new plot window for the square root of sensed voltage.
plot(appliedVoltage, sqrt(sensedVoltage), "-o", "LineWidth", 1.5); % Plot sqrt(sensed) against applied voltage.
grid on; % Show a grid to make voltage values easier to read.
xlabel("Applied Voltage (V)"); % Label the horizontal axis.
ylabel("sqrt(Sensed Voltage) (V^0.5)"); % Label the vertical axis.
title("sqrt(Sensed Voltage) vs. Applied Voltage"); % Describe the plotted relationship.
axis tight; % Adjust the axes limits to fit the data tightly.

% calculate Vth (threshold voltage) using the x-intercept of the fitted line
p = polyfit(appliedVoltage, sqrt(sensedVoltage), 1); % Fit a first-order polynomial (linear fit) to the square root of the sensed voltage.
% values return from p are the slope and y = mx + b line coefficients
% p(1) is the slope (m) and p(2) is the y-intercept (b) of the fitted line.
hold on; % Keep the current plot and add the fitted line.
plot(appliedVoltage, polyval(p, appliedVoltage), "r--", "LineWidth", 1.5); % Plot the fitted line in red.
legend("Measured Data", "Linear Fit"); % Add a legend to distinguish the data and fit.
fprintf("Fitted line coefficients: slope = %.3f, y-intercept = %.3f\n", p(1), p(2)); % Print the fitted line coefficients.

xintercept = -p(2)/p(1); % Calculate the x-intercept of the fitted line. Which is also the voltage threshold
fprintf("X-intercept of the fitted line: %.3f V\n", xintercept); % Print the x-intercept.