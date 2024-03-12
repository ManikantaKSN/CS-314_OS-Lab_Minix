import subprocess
import argparse
import matplotlib.pyplot as plt
from concurrent.futures import ThreadPoolExecutor, Future

parser = argparse.ArgumentParser(description='Process some integers.')
parser.add_argument('-s', type=int, help='an integer between 0 and 2000')
parser.add_argument('-n', type=int, default=20, help='number of runs')
parser.add_argument('-l', type=int, help='an integer between 0 and 2024')
parser.add_argument('-c', action='store_true', help='use -c option')

args = parser.parse_args()

x_values = []
y_values1 = []
y_values2 = []
y_values3 = []
y_values4 = []

def run_relocation(s, l) -> Future:
    future = Future()

    def do_work():
        cmd = ['python', 'relocation.py', '-s', str(s), '-n', ' 1000 ', '-l', str(l), '-c']
        if args.c:
            cmd.append('-c')
        output = subprocess.check_output(cmd)
        output_str = output.decode('utf-8')
        # print(output_str)
        valid_count = output_str.count('VALID')
        y = valid_count / 20
        future.set_result(y)

    executor.submit(do_work)
    return future

with ThreadPoolExecutor(max_workers=8) as executor:
    futures = [run_relocation(s, l) for s in range(0, 1) for l in range(0, 1025, 50)]
    for l, future in zip(range(0, 1025, 50), futures[::1]):
        y = future.result()
        x_values.append(l)
        # print(y)
        y_values1.append(y)
        # print(l)

with ThreadPoolExecutor(max_workers=8) as executor:
    futures = [run_relocation(s, l) for s in range(1,2) for l in range(0, 1025, 50)]
    for l, future in zip(range(0, 1025, 50), futures[::1]):
        y = future.result()
        # print(y)
        y_values2.append(y)
        # print(l)

with ThreadPoolExecutor(max_workers=8) as executor:
    futures = [run_relocation(s, l) for s in range(2, 3) for l in range(0, 1025, 50)]
    for l, future in zip(range(0, 1025, 50), futures[::1]):
        y = future.result()
        # print(y)
        y_values3.append(y)
        # print(l)

with ThreadPoolExecutor(max_workers=8) as executor:
    futures = [run_relocation(s, l) for s in range(3, 4) for l in range(0, 1025, 50)]
    for l, future in zip(range(0, 1025, 50), futures[::1]):
        y = future.result()
        # print(y)
        y_values4.append(y)
        # print(l)


plt.plot(x_values, y_values1, linestyle='-')
plt.plot(x_values, y_values2, linestyle='-')
plt.plot(x_values, y_values3, linestyle='-')
plt.plot(x_values, y_values4, linestyle='-')
plt.legend(['Seed 0','Seed 1','Seed 2', 'Seed 3'] )
plt.xlabel('Limit')
plt.ylabel('Fraction of Valid addresses')
plt.show()
# plt.savefig('output.png')