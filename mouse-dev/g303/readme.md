# Logitech G303 Apex

![G303](https://i.imgur.com/VaRJfzR.png)

The original firmware was read protected, so there's likely no going back to the manufacturers firmware. Scroll wheel is currently a little janky.

Erased and flashed with dapboot bootloader (STM32L100) with minor modifications.

* Keyboard Maintainer: [Dasky](https://github.com/Dasky)
* Hardware Supported: SteelSeries Prime+
* Hardware Availability: N/A

Make example for this keyboard (after setting up your build environment):

    make logitech/g303:default

Flashing example for this keyboard:

    make logitech/g303:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 3 ways:

* **Bootmagic reset**: Hold down left mouse button and plug in.
* **Bootloader entry**: Hold the CPI while plugging in.
