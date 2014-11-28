local disk = require "ldisk"

local disks = disk.get_disks()

local function printf(s, ...)
	return print(s:format(...))
end

printf("Found %d disk%s", #disks, #disks ~= 1 and "s" or "")

local prefixes = { [0] = "MiB", "GiB", "TiB" }
local function get_size_string(size)
	local prefix = 1
	for i, v in ipairs(prefixes) do
		local last_size = size
		size = size / 1024
		if size < 16 then
			prefix = i - 1
			size = last_size
			break
		end	
	end
	return size .. " " .. prefixes[prefix]
end

for i, v in ipairs(disks) do
	printf("Id: %d, Size: %s", v.id, get_size_string(v.size))
	for k, v in pairs(v) do
		print(k, v)
	end
	printf("-----------------------------------------")
end

