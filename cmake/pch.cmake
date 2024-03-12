SET(STD_PCH <cstdint>
          <vector>
          <random>
          <chrono>
          <array>
          <set>
          <unordered_map>
          <unordered_set>
          <algorithm>
          <type_traits>
          <deque>
        <concepts>
        <stdexcept>
        <string>
        <string_view>
        <numeric>
        <optional>
        <functional>
        <fstream>
        <filesystem>
        <stack>
        <span>
        <regex>
        <thread>
        <cstdlib>)

# --- functions --- #
function(use_pch PROJ)
  target_precompile_headers(${PROJ}
    PRIVATE
    ${STD_PCH}
  )
endfunction()