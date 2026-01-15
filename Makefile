# Nuked-OPL3 for z88dk - Makefile
# 
# z88dkを使用してビルドします

# コンパイラとツール
ZCC = zcc
RM = rm -f
MKDIR = mkdir -p

# ディレクトリ
SRC_DIR = src
INC_DIR = include
EXAMPLES_DIR = examples
BUILD_DIR = build

# 共通フラグ
COMMON_FLAGS = -vn -SO3 --max-allocs-per-node200000 -I$(INC_DIR)

# ソースファイル
OPL3_SRC = $(SRC_DIR)/opl3.c
EXAMPLE_SIMPLE = $(EXAMPLES_DIR)/simple_test.c

# ターゲット定義
.PHONY: all clean spectrum msx cpm amstrad help

# デフォルトターゲット
all: spectrum msx cpm

# ヘルプ
help:
	@echo "Nuked-OPL3 for z88dk ビルドシステム"
	@echo ""
	@echo "利用可能なターゲット:"
	@echo "  make spectrum   - ZX Spectrum用にビルド (.tap)"
	@echo "  make msx        - MSX用にビルド (.com)"
	@echo "  make cpm        - CP/M用にビルド (.com)"
	@echo "  make amstrad    - Amstrad CPC用にビルド (.cdt)"
	@echo "  make all        - すべてのターゲットをビルド"
	@echo "  make clean      - ビルド成果物を削除"
	@echo ""
	@echo "例:"
	@echo "  make spectrum         # ZX Spectrum版をビルド"
	@echo "  make spectrum OPTIMIZE=-O2  # 最適化レベル変更"

# ディレクトリ作成
$(BUILD_DIR):
	$(MKDIR) $(BUILD_DIR)

# ZX Spectrum (48K)
spectrum: $(BUILD_DIR)
	@echo "Building for ZX Spectrum..."
	$(ZCC) +zx $(COMMON_FLAGS) \
		-startup=31 \
		-clib=sdcc_iy \
		-pragma-include:zpragma.inc \
		-o $(BUILD_DIR)/opl3_spectrum.bin \
		$(OPL3_SRC) $(EXAMPLE_SIMPLE) \
		-create-app
	@echo "Created: $(BUILD_DIR)/opl3_spectrum.tap"

# ZX Spectrum 128K (より多くのRAM)
spectrum128: $(BUILD_DIR)
	@echo "Building for ZX Spectrum 128K..."
	$(ZCC) +zx $(COMMON_FLAGS) \
		-subtype=zx128 \
		-startup=31 \
		-clib=sdcc_iy \
		-o $(BUILD_DIR)/opl3_spectrum128.bin \
		$(OPL3_SRC) $(EXAMPLE_SIMPLE) \
		-create-app
	@echo "Created: $(BUILD_DIR)/opl3_spectrum128.tap"

# MSX
msx: $(BUILD_DIR)
	@echo "Building for MSX..."
	$(ZCC) +msx $(COMMON_FLAGS) \
		-subtype=msxdos \
		-clib=sdcc_iy \
		-o $(BUILD_DIR)/opl3_msx.com \
		$(OPL3_SRC) $(EXAMPLE_SIMPLE)
	@echo "Created: $(BUILD_DIR)/opl3_msx.com"

# CP/M
cpm: $(BUILD_DIR)
	@echo "Building for CP/M..."
	$(ZCC) +cpm $(COMMON_FLAGS) \
		-clib=sdcc_iy \
		-o $(BUILD_DIR)/opl3_cpm.com \
		$(OPL3_SRC) $(EXAMPLE_SIMPLE)
	@echo "Created: $(BUILD_DIR)/opl3_cpm.com"

# Amstrad CPC
amstrad: $(BUILD_DIR)
	@echo "Building for Amstrad CPC..."
	$(ZCC) +cpc $(COMMON_FLAGS) \
		-clib=sdcc_iy \
		-o $(BUILD_DIR)/opl3_amstrad.bin \
		$(OPL3_SRC) $(EXAMPLE_SIMPLE) \
		-create-app
	@echo "Created: $(BUILD_DIR)/opl3_amstrad.cdt"

# TRS-80
trs80: $(BUILD_DIR)
	@echo "Building for TRS-80..."
	$(ZCC) +trs80 $(COMMON_FLAGS) \
		-o $(BUILD_DIR)/opl3_trs80.cmd \
		$(OPL3_SRC) $(EXAMPLE_SIMPLE)
	@echo "Created: $(BUILD_DIR)/opl3_trs80.cmd"

# Sord M5
sordm5: $(BUILD_DIR)
	@echo "Building for Sord M5..."
	$(ZCC) +m5 $(COMMON_FLAGS) \
		-o $(BUILD_DIR)/opl3_m5.bin \
		$(OPL3_SRC) $(EXAMPLE_SIMPLE) \
		-create-app
	@echo "Created: $(BUILD_DIR)/opl3_m5.cas"

# コンパイラのアセンブリ出力を生成(デバッグ用)
asm: $(BUILD_DIR)
	@echo "Generating assembly output..."
	$(ZCC) +zx $(COMMON_FLAGS) \
		-a \
		-o $(BUILD_DIR)/opl3.asm \
		$(OPL3_SRC)
	@echo "Created: $(BUILD_DIR)/opl3.asm"

# ライブラリとしてビルド
library: $(BUILD_DIR)
	@echo "Building as library..."
	$(ZCC) +zx $(COMMON_FLAGS) \
		-c \
		-o $(BUILD_DIR)/opl3.o \
		$(OPL3_SRC)
	@echo "Created: $(BUILD_DIR)/opl3.o"

# クリーン
clean:
	@echo "Cleaning build artifacts..."
	$(RM) -r $(BUILD_DIR)
	$(RM) zcc_opt.def zpragma.inc
	@echo "Clean complete."

# サイズレポート
size:
	@echo "Checking binary sizes..."
	@ls -lh $(BUILD_DIR)/*.bin $(BUILD_DIR)/*.com 2>/dev/null || echo "No binaries found. Run 'make all' first."

# テスト(エミュレータが必要)
test-spectrum:
	@echo "Testing on ZX Spectrum emulator..."
	@if command -v fuse-emulator >/dev/null 2>&1; then \
		fuse-emulator $(BUILD_DIR)/opl3_spectrum.tap; \
	else \
		echo "Fuse emulator not found. Install with: apt-get install fuse-emulator-sdl"; \
	fi
