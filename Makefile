# ==============================================================================
# Variáveis de Configuração
# ==============================================================================

# Compilador C++ (usamos g++ que é compatível com OpenMP e a maioria dos sistemas Linux)
CXX = g++

# Flags Comuns: 
# -Wall: Habilita todos os avisos (boas práticas)
# -std=c++17: Utiliza o padrão C++17
# -O3: Nível máximo de otimização (necessário para projetos de alto desempenho)
CXXFLAGS = -Wall -std=c++17 -O3

# Flags para OpenMP (biblioteca de paralelismo)
OMPFLAGS = -fopenmp

# Diretórios
SRCDIR = src
COREDIR = $(SRCDIR)/core
SERIALDIR = $(SRCDIR)/serial
PARALLELDIR = $(SRCDIR)/parallel
BUILDDIR = build

# Arquivo executável de saída
TARGET = $(BUILDDIR)/parallel_clipper

# ==============================================================================
# Fontes e Objetos
# ==============================================================================

# Arquivos fonte do Core (compartilhados)
CORE_SRC = $(COREDIR)/Geometry.cpp $(COREDIR)/Polygon.cpp
CORE_OBJ = $(patsubst $(COREDIR)/%.cpp, $(BUILDDIR)/%.o, $(CORE_SRC))

# Arquivos fonte da Implementação Serial
SERIAL_SRC = $(SERIALDIR)/SequentialClipper.cpp
SERIAL_OBJ = $(patsubst $(SERIALDIR)/%.cpp, $(BUILDDIR)/%.o, $(SERIAL_SRC))

# Arquivos fonte da Implementação Paralela
PARALLEL_SRC = $(PARALLELDIR)/ParallelClipper.cpp
PARALLEL_OBJ = $(patsubst $(PARALLELDIR)/%.cpp, $(BUILDDIR)/%.o, $(PARALLEL_SRC))

# Arquivo fonte principal (main)
MAIN_SRC = $(SRCDIR)/main.cpp
MAIN_OBJ = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(MAIN_SRC))

# Todos os arquivos objeto necessários para o build final
ALL_OBJ = $(MAIN_OBJ) $(CORE_OBJ) $(SERIAL_OBJ) $(PARALLEL_OBJ)

# ==============================================================================
# Regras de Compilação
# ==============================================================================

.PHONY: all clean run

# Regra principal: Cria o diretório de build e compila o target
all: $(BUILDDIR) $(TARGET)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Regra para linkar (compila o executável)
# Nota: Incluímos $(OMPFLAGS) aqui, pois o projeto usa OpenMP na versão paralela
$(TARGET): $(ALL_OBJ)
	$(CXX) $(CXXFLAGS) $(ALL_OBJ) -o $@ $(OMPFLAGS)

# Regra para compilar o main.cpp
$(BUILDDIR)/main.o: $(MAIN_SRC)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regra genérica para compilar arquivos do core/
$(BUILDDIR)/%.o: $(COREDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regra genérica para compilar arquivos serial/
$(BUILDDIR)/%.o: $(SERIALDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regra genérica para compilar arquivos parallel/
# Nota: Compilamos com OMPFLAGS, pois o código paralelo utiliza essas diretivas
$(BUILDDIR)/%.o: $(PARALLELDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(OMPFLAGS) -c $< -o $@

# ==============================================================================
# Regras de Limpeza e Execução
# ==============================================================================

clean:
	rm -rf $(BUILDDIR)

# Exemplo de execução (modo serial no polígono de teste)
run_serial: all
	@echo "Executando o modo serial..."
	$(TARGET) serial data/input_convex_1k.poly data/output_serial.tri

# Exemplo de execução (modo paralelo com 4 threads - é necessário setar OMP_NUM_THREADS antes)
run_parallel: all
	@echo "Lembre-se de setar 'export OMP_NUM_THREADS=N' antes da execução."
	@echo "Executando o modo paralelo..."
	$(TARGET) parallel data/input_concave_10k.poly data/output_parallel.tri