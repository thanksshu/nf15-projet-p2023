"""
Generate page to block range mapping and page block to line range mapping
"""


class CarryCounter:
    def __init__(self, carry_when, initial_msb, initial_lsb) -> None:
        self._carry_when = carry_when
        self.msb = initial_msb
        self.lsb = initial_lsb

    def count(self):
        # Self increase, then carry if needed
        if self.lsb < self._carry_when - 1:
            self.lsb += 1
        else:
            self.msb += 1
            self.lsb = 0


class PageBlockRowMap:
    def __init__(self, page_count, page_size, block_size) -> None:
        self._page_count = page_count
        self._page_size = page_size
        self._block_size = block_size

        self._block_count = page_count * page_size // block_size
        self._margin = page_count * page_size % block_size // 2

        self.page_to_blocks = [
            [block_size - 1, 0] for _ in range(page_count)
        ]  # Get the needed block range for given page

        # Get the needed line range for given page and its block
        # Because of the margin, we need two more "virtual block" for the -1 block and the _self.block_count block
        # Fortunately, as we use the modulo operation, "+ 2" is just enough
        self.page_block_to_lines = [
            [[block_size - 1, 0] for _ in range(self._block_count + 2)]
            for _ in range(page_count)
        ]

        self._init_content()

    def _adjust_page_to_blocks_range(self, page, block):
        if block < self.page_to_blocks[page][0]:
            self.page_to_blocks[page][0] = block
        if block > self.page_to_blocks[page][1]:
            self.page_to_blocks[page][1] = block

    def _adjust_page_block_to_line_range(self, page, block, line):
        """调整 page_block_to_line_range 中的行范围"""
        _block = block + 1  # "+ 1" for the block that don't exist
        if line < self.page_block_to_lines[page][_block][0]:
            self.page_block_to_lines[page][_block][0] = line
        if line > self.page_block_to_lines[page][_block][1]:
            self.page_block_to_lines[page][_block][1] = line

    def _init_content(self):
        first_block = (
            0 * self._page_size - self._margin
        ) // self._block_size  # The starting page's first block
        first_block_line = (
            0 * self._page_size - self._margin
        ) % self._block_size  # And the first line

        # Iter over all block and page, record the mapping relationship
        block_counter = CarryCounter(self._block_size, first_block, first_block_line)
        for page in range(self._page_count):
            for _ in range(self._page_size):
                self._adjust_page_to_blocks_range(page, block_counter.msb)
                self._adjust_page_block_to_line_range(
                    page, block_counter.msb, block_counter.lsb
                )
                block_counter.count()

    def get_block_range(self, page):
        return self.page_to_blocks[page]

    def get_row_range(self, page, block):
        _block = block + 1  # "+ 1" for the block that don't exist
        return self.page_block_to_lines[page][_block]


if __name__ == "__main__":
    # Unit test
    from pprint import pprint

    BLOCK_SIZE = 12
    PAGE_SIZE = 8
    PAGE_COUNT = 8

    block_map = PageBlockRowMap(PAGE_COUNT, PAGE_SIZE, BLOCK_SIZE)
    pprint(block_map.page_to_blocks)
    pprint(block_map.page_block_to_lines)
