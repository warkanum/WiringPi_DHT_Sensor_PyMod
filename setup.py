from distutils.core import setup, Extension
import platform


def platform_correct(plat=platform.platform()):
    if plat.lower().find('armv6l') > -1:
        print("Platform: Rasberry Pi\n")
        return True
    if plat.lower().find('raspberry_pi') > -1:
        print("Platform: Rasberry Pi\n")
        return True
    if plat.lower().find('armv7l') > -1:
        print("Platform: Banana Pi\n")
        return True
    else:
        return False


if not platform_correct():
    print('Cannot build. You are required to have a Rasberry Pi or Bananna Pi platform.')
    exit(0)

module1 = Extension('dht11_sensor',
                    sources = ['_dht11_sensor.c','dht11_sensor.c']
                    ,libraries=['wiringPi']
                    ,extra_compile_args=['-std=gnu99'])

setup (name = 'WiringPi_DHT_Sensor',
       version = '1.0',
       author  = 'Hein Puth',
       author_email = 'hein.puth@gmail.com',
       license   = 'MIT',
       url  = 'http://www.kinathka.co.za',
       description = 'Python C module to read DHT Sensor with WiringPi',
       ext_modules = [module1])