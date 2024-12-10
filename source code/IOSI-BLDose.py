import numpy as np
import matplotlib.pyplot as plt
from scipy.ndimage import gaussian_filter

# 参数定义
num_particles = 100000  # 模拟粒子数量
volume_size = (100, 100, 100)  # 3D 体积大小 (x, y, z)
voxel_size = 0.5  # 每个体素的边长 (cm)
energy_per_particle = 1.0  # 每个粒子的初始能量 (MeV)

# 创建猪肺的密度分布 (伪 CT 数据)
def create_pig_lung_density(volume_size):
    density = np.ones(volume_size) * 0.3  # 基础肺密度 (g/cm³)
    # 模拟气管和血管区域
    z, y, x = np.indices(volume_size)
    center = np.array(volume_size) // 2
    r = 10  # 气管半径
    mask = (x - center[0]) ** 2 + (y - center[1]) ** 2 + (z - center[2]) ** 2 < r ** 2
    density[mask] = 0.05  # 气管密度 (g/cm³)

    # 添加更高密度区域代表血管
    for i in range(5):
        cx, cy, cz = np.random.randint(20, 80, size=3)
        r_vessel = np.random.randint(2, 5)
        vessel_mask = (x - cx) ** 2 + (y - cy) ** 2 + (z - cz) ** 2 < r_vessel ** 2
        density[vessel_mask] = 1.0  # 血管密度 (g/cm³)

    return density

# 初始化体积
lung_density = create_pig_lung_density(volume_size)
dose_distribution = np.zeros(volume_size)  # 剂量分布

# 模拟粒子传播与能量沉积
for _ in range(num_particles):
    # 粒子初始位置 (假设从中央点发射)
    position = np.array(volume_size) // 2
    energy = energy_per_particle

    while energy > 0:
        # 随机传播方向 (单位向量)
        direction = np.random.normal(size=3)
        direction /= np.linalg.norm(direction)

        # 移动到下一个体素
        step = np.random.exponential(scale=1.0)  # 路径长度 (cm)
        step_voxels = (direction * step / voxel_size).astype(int)
        position += step_voxels

        # 检查是否超出体积边界
        if not (0 <= position[0] < volume_size[0] and
                0 <= position[1] < volume_size[1] and
                0 <= position[2] < volume_size[2]):
            break

        # 获取当前体素的密度
        voxel_density = lung_density[tuple(position)]

        # 根据密度计算能量沉积
        deposited_energy = energy * 0.1 * voxel_density  # 简化能量沉积公式
        dose_distribution[tuple(position)] += deposited_energy

        # 剩余能量减少
        energy -= deposited_energy

# 将剂量转换为 Gy
voxel_mass = voxel_size ** 3 * lung_density  # 每个体素的质量 (g)
dose_distribution /= voxel_mass

# 平滑结果 (伪装真实物理扩散效应)
dose_distribution = gaussian_filter(dose_distribution, sigma=1)

# 可视化剂量分布
plt.figure(figsize=(10, 8))
plt.imshow(dose_distribution[:, :, volume_size[2] // 2], cmap="hot", origin="lower")
plt.colorbar(label="Dose (Gy)")
plt.title("Simulated Dose Distribution in Pig Lung")
plt.xlabel("X-axis (voxels)")
plt.ylabel("Y-axis (voxels)")
plt.show()
