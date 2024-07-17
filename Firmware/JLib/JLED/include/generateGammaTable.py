# stolen from : https://gist.github.com/hexagon5un/3df734ad08d8dc8d9ace0491ef97cc58#file-gamma-py
## Creates a gamma-corrected lookup table
import math
import os

def gamma(nsteps, gamma):
    gammaedUp = [math.pow(x, gamma) for x in range(nsteps)]
    return [x/max(gammaedUp) for x in gammaedUp]

def rounder(topValue, gammas):
    return [min(topValue, round(x*topValue)) for x in gammas]

if __name__ == "__main__":
    myGamma = 2.3
    steps = 256

    folderPath = os.path.dirname(os.path.realpath(__file__))
    with open(f"{folderPath}/GammaCorrection.hpp", "w") as output:
        output.write("#include <cstdint>\n\n")
        output.write(f"/* {steps}-step brightness table: gamma = {myGamma} */ \n")
        output.write("constexpr uint8_t gammaCorrectionLookup[%d] = {\n" % steps)
        for value in rounder(255, gamma(steps, myGamma)):
            output.write(" %d, " % value)
        output.write("\n};\n")


        output.write("\n/* raw gamma lookup (returns input value) */ \n")
        output.write("constexpr uint8_t gammaRawLookup[256] = {\n")
        for i in range(0, 256):
            output.write(" %d," % i)
        output.write("\n};\n")