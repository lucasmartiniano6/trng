#PERFORMS THE CHI-SQUARED TEST IN THE INPUT .BIN FILE, THE FILE SHOULD ONY CONTAIN 0's AND 1's (NO \n), ASSUMING ITS RANDOM THE PROBABILITY ITS EXPECTED TO BE 50% 1's AND 50% 0's, AND WE HAVE 1 DEGREE OF FREEDOM

def initialize(path):
    obsZer = 0 #OBSERVED ZEROS
    obsOne = 0 #OBSERVED ONES

    with open(path, 'rb') as f:
        data = f.read()

    for byte in data:
        for idx_bit in range(8):
            bit = byte & (1<<idx_bit)
            if bit:
                obsOne += 1
            else:
                obsZer += 1

    expected = (obsZer+obsOne)/2 #THE PROBABILTY EXPECTED IS 50-50, THEREFORE ITS HALF OF THE TOTAL FOR EACH BIT
    
    #BEGINNING OF MATHMETICAL CHI-SQUARED EXPRESSION
    chi0 = ((obsZer-expected)**2)/expected
    chi1 = ((obsOne-expected)**2)/expected

    chi = chi0 + chi1 #FINAL CHI-SQUARED VALUE

    results = {'observed zeros': obsZer,
                'observed ones': obsOne,
                'expected': expected,
                'total bits': (obsZer+obsOne),
                'CHI SQUARED': chi
                }

    return results
        

if __name__ == '__main__':
    import sys
    results = initialize(sys.argv[1])
    print(results)
