import subprocess, os.path


def test(inp, true_res, name=r'/a.out'):
    while True:
        inp_b = inp.encode('utf-8')
        fine_res = subprocess.run([os.getcwd() + name],
                                  input=inp_b, capture_output=True).stdout.decode("utf-8")
        if fine_res.strip(" \n") != true_res.strip(" \n"):
            print(inp)
            print(f'true_res = \n{true_res}')
            print(f'your_res = \n{fine_res}')
            if not input():
                break
