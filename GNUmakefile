libacpilib.a: acpi.o
	$(AR) rcs $@ $^

all: libacpilib.a \
	acpi-power \
	acpi-thermal \
	acpi-backlight \
	acpi-cpufreq

acpi-power: acpi-power.o -lacpilib

acpi-thermal: acpi-thermal.o -lacpilib

acpi-backlight: acpi-backlight.o -lacpilib

acpi-cpufreq: acpi-cpufreq.o -lacpilib

.PHONY: clean
clean:
	$(RM) *.o
	
.PHONY: distclean
distclean: clean
	$(RM) libacpilib.a \
		acpi-power \
		acpi-thermal \
		acpi-backlight \
		acpi-cpufreq

