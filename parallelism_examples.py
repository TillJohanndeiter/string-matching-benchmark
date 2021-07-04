import threading

from threading import Thread


def sequential(n=1000):
    for i in range(n):
        print(i)

    for i in range(n, n * 2):
        print(i)


def parallel(n=100000):
    def print_loop(low=0, up=n):
        for i in range(low, up):
            print(i)

    first_thread = Thread(target=print_loop, args=(0, n))
    second_thread = Thread(target=print_loop, args=(n, n * 2))
    first_thread.start()
    second_thread.start()


gaussian = 0


def concurrency(n=1000):
    global gaussian
    gaussian = 0

    print(f'Expected {(n * (n + 1)) / 2}')

    def add_to_sum(low, up):
        global gaussian
        for i in range(low, up):
            gaussian = gaussian + i


    half = n / 2
    first_thread = Thread(target=add_to_sum, args=(0, half))
    second_thread = Thread(target=add_to_sum, args=(half, n))
    first_thread.start()
    second_thread.start()
    first_thread.join()
    second_thread.join()


    print(f'Actual Sum: {gaussian}')

lock = threading.Lock()

def mutex_example(n=1000):
    global gaussian
    gaussian = 0
    print(f'Expected {(n * (n + 1)) / 2}')

    def add_to_sum(low, up):
        global gaussian
        for i in range(low, up):
            lock.acquire()
            gaussian = gaussian + i
            lock.release()

    half = int(n / 2)
    first_thread = Thread(target=add_to_sum, args=(0, half))
    second_thread = Thread(target=add_to_sum, args=(half, n))
    first_thread.start()
    second_thread.start()

    first_thread.join()
    second_thread.join()

    print(f'Actual Sum: {gaussian}')


if __name__ == '__main__':
    mutex_example(10000000)
