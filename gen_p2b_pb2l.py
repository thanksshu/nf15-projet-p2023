"""
Generate Page to blocks mapping and Page with block to lines mapping (aka P2B_MAP and PB2L_MAP)
"""


class CarryCounter:
    def __init__(self, carry_when, initial_msb, initial_lsb) -> None:
        self._carry_when = carry_when
        self.msb = initial_msb
        self.lsb = initial_lsb

    def count(self):
        # Increase lsb once, then carry if possible
        if self.lsb < self._carry_when - 1:
            self.lsb += 1
        else:
            self.msb += 1
            self.lsb = 0


class QueryTable:
    def __init__(self, page_count, page_size, block_size) -> None:
        self._page_count = page_count
        self._page_size = page_size
        self._block_size = block_size

        self._block_count = page_count * page_size // block_size
        self._margin = page_count * page_size % block_size // 2

        self._page_to_blocks = [
            [block_size - 1, 0] for _ in range(page_count)
        ]  # Block range for the given page

        # Line range for the given page and block
        # The "+2" is caused by margins; btw, due to the algorithm, "+2" is just enough.
        self._page_block_to_lines = [
            [[block_size - 1, 0] for _ in range(self._block_count + 2)]
            for _ in range(page_count)
        ]

        self._init_content()

    def _adjust_page_to_blocks_range(self, page, block):
        if block < self._page_to_blocks[page][0]:
            self._page_to_blocks[page][0] = block
        if block > self._page_to_blocks[page][1]:
            self._page_to_blocks[page][1] = block

    def _adjust_page_block_to_line_range(self, page, block, line):
        _block = block + 1  # Treat the -1 block
        if line < self._page_block_to_lines[page][_block][0]:
            self._page_block_to_lines[page][_block][0] = line
        if line > self._page_block_to_lines[page][_block][1]:
            self._page_block_to_lines[page][_block][1] = line

    def _init_content(self):
        first_block = (
            0 * self._page_size - self._margin
        ) // self._block_size  # Block at page 0 and page line 0
        first_block_line = (
            0 * self._page_size - self._margin
        ) % self._block_size  # The line of block at page 0 and page line 0

        # Iter over both block and page, adjust these two maps
        block_counter = CarryCounter(self._block_size, first_block, first_block_line)
        for page in range(self._page_count):
            for _ in range(self._page_size):
                self._adjust_page_to_blocks_range(page, block_counter.msb)
                self._adjust_page_block_to_line_range(
                    page, block_counter.msb, block_counter.lsb
                )
                block_counter.count()

    def get_block_range(self, page):
        return self._page_to_blocks[page]

    def get_line_range(self, page, block):
        _block = block + 1  # treat the -1 block
        return self._page_block_to_lines[page][_block]


if __name__ == "__main__":
    from pprint import pprint

    BLOCK_SIZE = 12
    PAGE_SIZE = 8
    PAGE_COUNT = 8

    block_map = QueryTable(PAGE_COUNT, PAGE_SIZE, BLOCK_SIZE)
    print("Page to blocks mapping: ")
    pprint(block_map._page_to_blocks)
    print("Page with block to lines mapping: ")
    pprint(block_map._page_block_to_lines)
