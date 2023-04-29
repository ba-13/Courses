# we have a single block in context, with 177 pages in it
# we'll choose which page to use by static wear levelling algorithm
import numpy as np
from fsm import FSM


class Cell:
    def __init__(self, idx: int, string: int, block: int):
        self.page = idx
        self.string = string
        self.block = block
        self.state = 1
        self.usage = 0

    def __repr__(self):
        return "Cell(%d, %d)->%d" % (self.page, self.string, self.state)

    def unsetBit(self):
        flag = 0
        if self.state == 1:
            self.usage += 1
            flag = 1
        self.state = 0
        return flag

    def setBit(self):
        flag = 0
        if self.state == 0:
            self.usage += 1
            flag = 1
        self.state = 1
        return flag

    def apply_vg(self, vg: int):
        if vg == 2.5:
            return self.state
        elif vg == 8:
            return 1


class String:
    def __init__(self, idx: int, length: int, block: int) -> None:
        self.idx = idx
        self.length = length
        self.block = block
        self.enable = 0
        self.cells = [Cell(i, idx, block) for i in range(length)]

    def output(self, Vg: np.ndarray) -> int:
        if self.enable == 0:
            return 0
        out_ = 1
        for page, cell in enumerate(self.cells):
            if page in Vg:
                out_ = cell.apply_vg(2.5)
            else:
                out_ = cell.apply_vg(8)
            if out_ == 0:
                return 0
        return 1


class Block:
    def __init__(self, idx: int, num_strings: int, correct_key: int) -> None:
        self.num_pages = 177
        self.idx = idx
        self.correct_key = correct_key
        self.strings = [String(i, self.num_pages, idx) for i in range(num_strings)]
        self.usage_count = np.zeros((self.num_pages, num_strings))
        self.fsm = iter(
            FSM(num_pages=self.num_pages, seed=correct_key)
        )  # seed set by designer
        # this fsm is for setting input locations
        self.enabled_strings = set()

    def reset(self) -> None:
        self.fsm = iter(FSM(num_pages=self.num_pages, seed=self.correct_key))

    def output(self, Vg: set) -> int:
        for i, string in enumerate(self.strings):
            out_ = string.output(Vg)
            if out_ == 1:
                return 1
        return 0

    def disable_strings(self) -> None:
        for enabled_string in self.enabled_strings:
            self.strings[enabled_string].enable = 0
        self.enabled_strings = set()

    def clear_block(self) -> None:
        self.enabled_strings = set()
        for string_idx, _ in enumerate(self.strings):
            self.strings[string_idx].enable = 0
            for cell in self.strings[string_idx].cells:
                self.usage_count[cell.page][string_idx] += cell.setBit()

    def set_cell(self, page: int, string: int) -> None:
        self.strings[string].enable = 1
        self.enabled_strings.add(string)
        self.usage_count[page][string] += self.strings[string].cells[page].setBit()

    def unset_cell(self, page: int, string: int) -> None:
        self.strings[string].enable = 1
        self.enabled_strings.add(string)
        self.usage_count[page][string] += self.strings[string].cells[page].unsetBit()


class Function:
    def __init__(self, block: Block) -> None:
        self.block = block

    def write_to_block(self, func: list) -> list:
        self.minimum_used_strings = np.argsort(np.sum(self.block.usage_count, axis=0))
        # print("minimum_used_strings:", self.minimum_used_strings)
        pages_used = set()
        # count all pages to be used
        j = 0
        while j < func[0][0]:
            next_page = self.block.fsm.__next__()
            prev_size = len(pages_used)
            pages_used.add(next_page)
            if len(pages_used) == prev_size:  # if collision occurred in pages_used
                j -= 1
            j += 1
        j = 0
        while j < func[0][1]:
            next_page = self.block.fsm.__next__()
            prev_size = len(pages_used)
            pages_used.add(next_page)
            if len(pages_used) == prev_size:  # if collision occurred in pages_used
                j -= 1
            j += 1
        pages_used = list(pages_used)

        for i, string in enumerate(func):
            string_id = self.minimum_used_strings[i]
            page_using_curr = 0
            zeros = string[0]
            ones = string[1]
            while zeros > 0:
                next_page = pages_used[page_using_curr]
                self.block.unset_cell(next_page, int(string_id))
                page_using_curr += 1
                zeros -= 1
            while ones > 0:
                next_page = pages_used[page_using_curr]
                self.block.set_cell(next_page, int(string_id))
                page_using_curr += 1
                ones -= 1
            while page_using_curr < len(pages_used):
                next_page = pages_used[page_using_curr]
                self.block.set_cell(next_page, int(string_id))
                page_using_curr += 1

        return pages_used


class Oracle:
    def __init__(self) -> None:
        pass

    def true_value(self, func: list):
        for string in func:
            if string[0] > 0:
                continue
            else:
                return 1
        return 0


class KeyApply:
    def __init__(self, key) -> None:
        self.fsm = iter(FSM(num_pages=177, seed=key))

    def apply(self, func: list) -> set:
        Vg_vector = set()
        j = 0
        while j < func[0][0]:
            next_page = self.fsm.__next__()
            prev_size = len(Vg_vector)
            Vg_vector.add(next_page)
            if len(Vg_vector) == prev_size:  # if collision occurred in Vg_vector
                j -= 1
            j += 1
        j = 0
        while j < func[0][1]:
            next_page = self.fsm.__next__()
            prev_size = len(Vg_vector)
            Vg_vector.add(next_page)
            if len(Vg_vector) == prev_size:  # if collision occurred in Vg_vector
                j -= 1
            j += 1
        return Vg_vector

    def reinit(self, key):
        self.fsm = iter(FSM(num_pages=177, seed=key))


class FlashModule:
    def __init__(self, key, correct_key, num_strings) -> None:
        self.debug = False
        self.block = Block(idx=0, num_strings=num_strings, correct_key=correct_key)
        self.keyHandler = KeyApply(key)
        self.oracle = Oracle()
        self.function = Function(self.block)

    def write(self, func: list):
        func.sort(key=lambda x: x[0] + x[1], reverse=True)
        # self.block.clear_block()
        self.block.disable_strings()
        pages_used = self.function.write_to_block(func)
        if self.debug:
            print(f"Pages Used: {pages_used}")
        Vg = self.keyHandler.apply(func)
        if self.debug:
            print("Vg vector:", Vg)
        Y = self.oracle.true_value(func)
        Yhat = self.block.output(Vg)
        return Y, Yhat

    def reinit(self, key):
        self.block.reset()
        self.keyHandler.reinit(key)
