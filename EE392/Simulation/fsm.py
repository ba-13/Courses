class FSM:
    def __init__(self, num_pages, seed) -> None:
        self.num_pages = num_pages
        self.a = 1664525  # a-1 should be divisible by all prime factors of num_pages
        self.c = 1013904223
        self.m = 2**32
        self.seed = seed  # maybe sum of bits of Vg init
        self.step = 0

    def __iter__(self):
        return self

    def __next__(self):
        self.seed = (self.a * self.seed + self.c) % self.m
        self.step += 1
        return self.seed % self.num_pages


if __name__ == "__main__":
    fsm = iter(FSM(177, 155))
    fsm2 = iter(FSM(177, 155))
    found = []
    while True:
        next_ = fsm.__next__()
        next2_ = fsm2.__next__()
        print(next_, next2_)
        if len(found) > 1000:
            break
        found.append(next_)
    print(len(found))
